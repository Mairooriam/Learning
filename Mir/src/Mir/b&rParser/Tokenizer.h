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
    std::vector<Token> tokenize();

private:
    std::string m_source;
    size_t position = 0;
    int line = 1;
    int column = 1;

    static const std::unordered_set<std::string> keywords;
    static const std::unordered_set<std::string> dataTypes;
    static const std::unordered_set<std::string> boolLiterals;

    char current() const;
    char peek(int offset = 1) const;
    void advance();
    Token nextToken();
    Token scanComment();
    Token scanIdentifierOrKeyword();
    Token scanProcessVariable();
    bool isAtEnd() const;
    void skipWhitespace();
    std::string toLower(const std::string& s) const;

    std::vector<Token> tokenizeCSV();
    std::vector<Token> tokenizeGeneral();
    std::vector<std::string> splitCsvLine(const std::string& line, const char delimeter);
};

} // tokenizer ns
} // mir ns