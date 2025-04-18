#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <memory>

namespace Mir {
namespace Tokenizer{



enum class Keyword {
    TYPE,
    END_TYPE,
    STRUCT,
    END_STRUCT,
    VAR,
    END_VAR,
    VAR_CONFIG,
    AT,
    UNKNOWN
};


enum class TokenType {
    Keyword,        // TYPE, STRUCT, END_STRUCT, END_TYPE
    Identifier,     // Custom names
    DataType,       // USINT, BOOL, etc.
    Comment,        // (*comment*)
    Colon,          // :
    DoubleColon,    // ::
    Semicolon,      // ;
    Dot,            // .
    Assignment,     // :=
    BoolLiteral,    // TRUE, FALSE
    EOF_Token,      // End of file
    ProcessVariable,    // For "::Tag.Member" syntax
    HardwareType,       // For "%IX", "%QX", etc.
    QuotedString,       // For quoted module paths like "AF103"
    QuotedIdentifier,     // "AF102"
    IoAddress,      // For complete I/O addresses  
    CSV_LOCATION,    //CC_1
    CSV_TYPE,
    CSV_NAME,
    CSV_FIELD,
    CSV_CARD,
    CSV_HEADER,
    CSV_EPLANNAME,
    TemplateOpen,      // {
    TemplateClose,     // }
    TemplateVariable,  // location, name, etc. inside braces
    TemplateText,   
    Unknown         // Unrecognized token
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
    Keyword keywordType;

    Token(TokenType t, const std::string& v, int l, int c)
        : type(t), value(v), line(l), column(c), keywordType(Keyword::UNKNOWN) {
        if (type == TokenType::Keyword) {
            keywordType = stringToKeyword(v);
        }
    }
    static Keyword stringToKeyword(const std::string& str) {
        std::string upper = str;
        std::transform(upper.begin(), upper.end(), upper.begin(), 
                      [](unsigned char c){ return std::toupper(c); });

        if (upper == "TYPE") return Keyword::TYPE;
        if (upper == "END_TYPE") return Keyword::END_TYPE;
        if (upper == "STRUCT") return Keyword::STRUCT;
        if (upper == "END_STRUCT") return Keyword::END_STRUCT;
        if (upper == "VAR") return Keyword::VAR;
        if (upper == "END_VAR") return Keyword::END_VAR;
        if (upper == "VAR_CONFIG") return Keyword::VAR_CONFIG;
        if (upper == "AT") return Keyword::AT;
        return Keyword::UNKNOWN;
    }
    
    std::string toString() const {
        std::string typeStr;
        switch (type) {
            case TokenType::Keyword: typeStr = "Keyword"; break;
            case TokenType::Identifier: typeStr = "Identifier"; break;
            case TokenType::DataType: typeStr = "DataType"; break;
            case TokenType::Comment: typeStr = "Comment"; break;
            case TokenType::Colon: typeStr = "Colon"; break;
            case TokenType::Semicolon: typeStr = "Semicolon"; break;
            case TokenType::Dot: typeStr = "Dot"; break;
            case TokenType::DoubleColon: typeStr  = "DoubleDot"; break;
            case TokenType::Assignment: typeStr = "Assignment"; break;
            case TokenType::BoolLiteral: typeStr = "BoolLiteral"; break;
            case TokenType::EOF_Token: typeStr = "EOF"; break;
            case TokenType::HardwareType: typeStr = "HardwareType"; break;
            case TokenType::QuotedIdentifier: typeStr = "QuotedIdentifier"; break;
            case TokenType::CSV_LOCATION: typeStr = "CSV_LOCATION"; break;
            case TokenType::CSV_TYPE: typeStr = "CSV_TYPE"; break;
            case TokenType::CSV_NAME: typeStr = "CSV_NAME"; break;
            case TokenType::CSV_FIELD: typeStr = "CSV_FIELD"; break;
            case TokenType::CSV_CARD: typeStr = "CSV_CARD"; break;
            case TokenType::TemplateOpen: typeStr = "TemplateOpen"; break;
            case TokenType::TemplateClose: typeStr = "TemplateClose"; break;
            case TokenType::TemplateVariable: typeStr = "TemplateVariable"; break;
            case TokenType::TemplateText: typeStr = "TemplateText"; break;
            case TokenType::CSV_HEADER: typeStr = "CSV_HEADER"; break;
            case TokenType::CSV_EPLANNAME: typeStr = "CSV_EPLANNAME"; break;
            case TokenType::IoAddress: typeStr = "IoAddress"; break;
            case TokenType::QuotedString: typeStr = "QuotedString"; break;
            case TokenType::Unknown: typeStr = "Unknown"; break;
            case TokenType::ProcessVariable: typeStr = "ProcessVariable"; break;

        }
        return typeStr + ": '" + value + "' at Line " + std::to_string(line) + ", Column " + std::to_string(column);
    }
};

class Tokenizer {
public:
    Tokenizer(const std::string& source);
    virtual std::vector<Token> tokenize();

protected:
    std::string m_source;
    size_t position = 0;
    int line = 1;
    int column = 1;
    bool isAtEnd() const;
    char peek(int offset = 1) const;
    char current() const;
    void advance();

private:

    static const std::unordered_set<std::string> keywords;
    static const std::unordered_set<std::string> dataTypes;
    static const std::unordered_set<std::string> boolLiterals;

    Token nextToken();
    Token scanComment();
    Token scanIdentifierOrKeyword();
    void skipWhitespace();
    std::string toLower(const std::string& s) const;


    std::vector<Token> tokenizeGeneral();

};

class TemplateTokenizer : public Tokenizer {
    private:
        std::unordered_set<std::string> m_templateVars;
    public:
        TemplateTokenizer(const std::string& templateStr, const std::vector<std::string>& templateVars) : Tokenizer(templateStr) {
            // just for nicer syntax on lookup
            m_templateVars = std::unordered_set<std::string>(templateVars.begin(), templateVars.end());
        }
        
        std::vector<Token> tokenize() override {
            std::vector<Token> tokens;
            
            position = 0;
            line = 1;
            column = 1;
            
            while (!isAtEnd()) {
                char c = current();
                
                if (c == '{') {
                    // For now dont need to tokenize the brackets
                    //tokens.push_back(Token(TokenType::TemplateOpen, "{", line, column));
                    advance();
                    
                    size_t startPos = position;
                    int startCol = column;
                    
                    while (!isAtEnd() && current() != '}') {
                        advance();
                    }
                    
                    if (isAtEnd()) {
                        tokens.push_back(Token(TokenType::Unknown, "Unclosed template variable", line, startCol));
                        break;
                    }
                    
                    std::string varName = m_source.substr(startPos, position - startPos);
                    if (m_templateVars.find(varName) != m_templateVars.end() )
                    {
                        tokens.push_back(Token(TokenType::TemplateVariable, varName, line, startCol));
                    } else {
                        tokens.push_back(Token(TokenType::Unknown, "Not valid variable", line, startCol));
                    }
                    
                    
                    // For now dont need to tokenize the brackets
                    //tokens.push_back(Token(TokenType::TemplateClose, "}", line, column));
                    advance();
                } 
                else {
                    size_t startPos = position;
                    int startCol = column;
                    
                    while (!isAtEnd() && current() != '{') {
                        advance();
                    }
                    
                    std::string text = m_source.substr(startPos, position - startPos);
                    if (!text.empty()) {
                        tokens.push_back(Token(TokenType::TemplateText, text, line, startCol));
                    }
                }
            }
            
            return tokens;
        }
    };


} // tokenizer ns
} // mir ns