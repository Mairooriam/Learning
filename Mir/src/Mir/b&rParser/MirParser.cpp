#include "MirParser.h"
#include "Core.h"
namespace Mir {
    namespace Parser{
    MirParser::MirParser(const std::vector<Tokenizer::Token> &tokens)
    :m_Tokens(tokens){ 
        
        skipUntil(Tokenizer::TokenType::Keyword, false);
        std::string value = m_Tokens[m_CurrentPosition].value;
        if (value == "TYPE") {
            m_FileFormat = FileFormat::TYP;
            m_CurrentPosition = 0;
            parse();
        }
        else if (value == "VAR_CONFIG") {
            m_FileFormat = FileFormat::IOM;
        }
        else {
            MIR_ASSERT(false,"NOT IMPLEMENTED UNKOWN")
        }
        m_CurrentPosition = 0;
    }

    ParseResult MirParser::parse()
    {
        ParseResult result;
        if (m_FileFormat == FileFormat::TYP)
        {
            result.typDefinitions = parseTypFile();
            // parseTypTypeBlock();
            // parseTypStruct();
            // parseTypMember();
        }

        // if (m_FileFormat == FileFormat::IOM)
        // {
        //     /* code */
        // }
        return result;
    }

    Tokenizer::Token MirParser::peek() const {
        if (isAtEnd()) {
            return Tokenizer::Token(Tokenizer::TokenType::EOF_Token, "", 0, 0);
        }
        return m_Tokens[m_CurrentPosition];
    }

    Tokenizer::Token MirParser::peekAhead(int n) const {
        if (m_CurrentPosition + n >= m_Tokens.size()) {
            return Tokenizer::Token(Tokenizer::TokenType::EOF_Token, "", 0, 0);
        }
        return m_Tokens[m_CurrentPosition + n];
    }

    Tokenizer::Token MirParser::advance() {
        if (!isAtEnd()) {
            m_CurrentPosition++;
        }
        return m_Tokens[m_CurrentPosition - 1];
    }
    bool MirParser::match(Tokenizer::TokenType type) {
        if (isAtEnd()) return false;
        if (peek().type != type) return false;
        
        advance();
        return true;
    }

    void MirParser::skipUntil(Tokenizer::TokenType type, bool skipMatchedToken) {
        while (!isAtEnd() && peek().type != type) {
            advance();
        }
        
        // Skip the matched token if requested
        if (!isAtEnd() && skipMatchedToken) {
            advance();
        }
    }

    MemberDefinition MirParser::parseTypMember()
    {
        MemberDefinition result;
        
        // We expect to be at an identifier (member name)
        if (peek().type == Tokenizer::TokenType::Identifier) {
            result.name = peek().value;
            advance();
            
            // Check for colon
            if (match(Tokenizer::TokenType::Colon)) {
                // Parse data type
                if (peek().type == Tokenizer::TokenType::DataType || 
                    peek().type == Tokenizer::TokenType::Identifier) {
                    result.type = peek().value;
                    advance();
                }
                
                // Check for assignment
                if (match(Tokenizer::TokenType::Assignment)) {
                    // Parse value
                    if (peek().type == Tokenizer::TokenType::BoolLiteral) {
                        result.value = peek().value;
                        advance();
                    }
                }
            }
            
            // Check for comment
            if (peek().type == Tokenizer::TokenType::Comment) {
                result.comment = peek().value;
                advance();
            }
            
            // Skip semicolon
            if (match(Tokenizer::TokenType::Semicolon)) {
                // Semicolon consumed
            }
        }
        
        return result;
    }

    StructDefinition MirParser::parseTypStruct()
    {
        StructDefinition result;
        
        // We expect to be at an identifier (struct name)
        if (peek().type == Tokenizer::TokenType::Identifier) {
            result.name = peek().value;
            advance();
            
            // Check for colon
            if (match(Tokenizer::TokenType::Colon)) {
                // Look for STRUCT keyword
                if (peek().type == Tokenizer::TokenType::Keyword && 
                    peek().keywordType == Tokenizer::Keyword::STRUCT) {
                    
                    advance(); // Skip STRUCT keyword
                    
                    // Check for comment after STRUCT
                    if (peek().type == Tokenizer::TokenType::Comment) {
                        result.comment = peek().value;
                        advance();
                    }
                    
                    // Parse members until END_STRUCT
                    while (!isAtEnd() && 
                           !(peek().type == Tokenizer::TokenType::Keyword && 
                             peek().keywordType == Tokenizer::Keyword::END_STRUCT)) {
                        
                        if (peek().type == Tokenizer::TokenType::Identifier) {
                            MemberDefinition member = parseTypMember();
                            result.members.push_back(member);
                        } else {
                            advance(); // Skip unexpected tokens
                        }
                    }
                    
                    // Skip END_STRUCT
                    if (peek().type == Tokenizer::TokenType::Keyword && 
                        peek().keywordType == Tokenizer::Keyword::END_STRUCT) {
                        advance();
                        
                        // Skip optional semicolon after END_STRUCT
                        if (peek().type == Tokenizer::TokenType::Semicolon) {
                            advance();
                        }
                    }
                }
            }
        }
        
        return result;
    }

    std::vector<TypeDefinition> MirParser::parseTypFile()
    {
        std::vector<TypeDefinition> result;
        
        // Reset position to start from beginning
        m_CurrentPosition = 0;
        
        while (!isAtEnd()) {
            TypeDefinition typeDefinition;
            
            // Look for comments before TYPE
            if (peek().type == Tokenizer::TokenType::Comment) {
                typeDefinition.comment = peek().value;
                advance();
            }
            
            // Look for TYPE keyword
            if (peek().type == Tokenizer::TokenType::Keyword && 
                peek().keywordType == Tokenizer::Keyword::TYPE) {
                
                advance(); // Skip TYPE keyword
                
                // Parse structs until END_TYPE
                while (!isAtEnd() && 
                      !(peek().type == Tokenizer::TokenType::Keyword && 
                        peek().keywordType == Tokenizer::Keyword::END_TYPE)) {
                    
                    // Check for struct definition
                    if (peek().type == Tokenizer::TokenType::Identifier) {
                        StructDefinition structDef = parseTypStruct();
                        if (!structDef.name.empty()) {
                            typeDefinition.structs.push_back(structDef);
                        }
                    } else {
                        // Skip anything that's not an identifier
                        advance();
                    }
                }
                
                // Skip END_TYPE
                if (peek().type == Tokenizer::TokenType::Keyword && 
                    peek().keywordType == Tokenizer::Keyword::END_TYPE) {
                    advance();
                }
                
                // Add type definition to results
                if (!typeDefinition.structs.empty()) {
                    result.push_back(typeDefinition);
                }
            } else {
                // Skip tokens until we find TYPE or EOF
                advance();
            }
        }
        
        return result;
    }
}
}