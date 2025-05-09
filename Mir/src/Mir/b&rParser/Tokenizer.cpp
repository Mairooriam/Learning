#include "Tokenizer.h"
#include <algorithm>
#include <cctype>

namespace Mir {
namespace Tokenizer{

const std::unordered_set<std::string> Tokenizer::keywords = {
    "type", "struct", "end_struct", "end_type", "var_config", "end_var", "at"
};

const std::unordered_set<std::string> Tokenizer::dataTypes = {
    "usint", "bool", "int", "uint", "dint", "udint", "real", "lreal", "string", "time"
    // Add more data types as needed
};

const std::unordered_set<std::string> Tokenizer::boolLiterals = {
    "true", "false"
};

Tokenizer::Tokenizer(const std::string& source) : m_source(source) {}

char Tokenizer::current() const {
    if (isAtEnd()) return '\0';
    return m_source[position];
}

char Tokenizer::peek(int offset) const {
    if (position + offset >= m_source.length()) return '\0';
    return m_source[position + offset];
}

void Tokenizer::advance() {
    if (current() == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    position++;
}

bool Tokenizer::isAtEnd() const {
    return position >= m_source.length();
}

void Tokenizer::skipWhitespace() {
    while (!isAtEnd() && std::isspace(current())) {
        advance();
    }
}

std::string Tokenizer::toLower(const std::string& s) const {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return result;
}

// std::vector<Token> Tokenizer::tokenizeCSV() {
//     std::vector<Token> tokens;
    
//     std::istringstream stream(m_source);
//     std::string line;
//       std::vector<std::string> headers;
//     int lineNumber = 1;

//     // get header
//     std::getline(stream, line);
//     if (lineNumber == 1 && !line.empty())
//     {
//         headers = splitCsvLine(line, ',');
//         int column = 1;
//         for (const std::string& header : headers)
//         {
//             tokens.push_back(Token(TokenType::CSV_HEADER, header, lineNumber, column));
//             column++;
//         }
        
        

//         lineNumber++;
        
//     }

//     // Fields
//     while (std::getline(stream, line)) {
//         std::vector<std::string> fields = splitCsvLine(line, ',');
//         int column = 1;
//         for (const std::string& field : fields)
//         {
//             tokens.push_back(Token(TokenType::CSV_FIELD, field, lineNumber, column));
//             column++;
//         }

//         lineNumber++;
//     }
    
//     tokens.push_back(Token(TokenType::EOF_Token, "", lineNumber, 1));
//     return tokens;
// }
std::vector<Token> Tokenizer::tokenizeGeneral()
{
    std::vector<Token> tokens;
    while (!isAtEnd()) {
        Token token = nextToken();
        tokens.push_back(token);
        
        if (token.type == TokenType::EOF_Token) {
            break;
        }
    }
    return tokens;
}
// std::vector<std::string> Tokenizer::splitCsvLine(const std::string &line, const char delimeter)
// {
//     std::vector<std::string> fields;
//     std::string field;
//     std::istringstream lineStream(line);

//     bool inQuotes = false;
//     for (size_t i = 0; i < line.length(); ++i) {
//         char c = line[i];
        
//         if (c == '"') {
//             inQuotes = !inQuotes;
//         } else if (c == delimeter && !inQuotes) {
//             fields.push_back(field);
//             field.clear();
//         } else {
//             field += c;
//         }
//     }

//     fields.push_back(field);
    
//     return fields;
// }
Token Tokenizer::scanComment()
{
    int startLine = line;
    int startColumn = column;
    std::string value = "";
    
    // Skip the opening (*
    value += current();
    advance();
    value += current();
    advance();
    
    // Scan until closing *)
    while (!isAtEnd() && !(current() == '*' && peek() == ')')) {
        value += current();
        advance();
    }
    
    if (!isAtEnd()) {
        // Add closing *)
        value += current(); // *
        advance();
        value += current(); // )
        advance();
    }
    
    return Token(TokenType::Comment, value, startLine, startColumn);
}

Token Tokenizer::scanIdentifierOrKeyword() {
    int startLine = line;
    int startColumn = column;
    std::string value = "";
    
    // Collect the identifier
    while (!isAtEnd() && (std::isalnum(current()) || current() == '_')) {
        value += current();
        advance();
    }
    
    // Check if it's a keyword, data type, or boolean literal
    std::string lowerValue = toLower(value);
    
    if (keywords.find(lowerValue) != keywords.end()) {
        return Token(TokenType::Keyword, value, startLine, startColumn);
    } else if (dataTypes.find(lowerValue) != dataTypes.end()) {
        return Token(TokenType::DataType, value, startLine, startColumn);
    } else if (boolLiterals.find(lowerValue) != boolLiterals.end()) {
        return Token(TokenType::BoolLiteral, value, startLine, startColumn);
    } else {
        return Token(TokenType::Identifier, value, startLine, startColumn);
    }
}

Token Tokenizer::nextToken() {
    skipWhitespace();
    
    if (isAtEnd()) {
        return Token(TokenType::EOF_Token, "", line, column);
    }
    
    int startLine = line;
    int startColumn = column;
    
    // Check for comments
    if (current() == '(' && peek() == '*') {
        return scanComment();
    }
    
    // Check for single-line comments
    if (current() == '/' && peek() == '/') {
        std::string value = "";
        
        // Collect until end of line
        while (!isAtEnd() && current() != '\n') {
            value += current();
            advance();
        }
        
        return Token(TokenType::Comment, value, startLine, startColumn);
    }
    
    // Check for identifiers or keywords
    if (std::isalpha(current()) || current() == '_') {
        return scanIdentifierOrKeyword();
    }
    
    // Check for symbols and operators
    switch (current()) {
        case ':': {
            advance();
            if (current() == '=') {
                advance();
                return Token(TokenType::Assignment, ":=", startLine, startColumn);
            } else if (current() == ':') {
                advance();
                return Token(TokenType::DoubleColon, "::", startLine, startColumn);
            }
            return Token(TokenType::Colon, ":", startLine, startColumn);
        }
        case ';': {
            advance();
            return Token(TokenType::Semicolon, ";", startLine, startColumn);
        }
        case '.': {
            advance();
            return Token(TokenType::Dot, ".", startLine, startColumn);
        }case '"': {
            std::string value = "\""; 
            advance();
            int quoteStartLine = line;
            int quoteStartColumn = column;
            
            // Collect all characters until the closing quote
            while (!isAtEnd() && current() != '"') {
                value += current();
                advance();
            }
            
            if (isAtEnd()) {
                MIR_ASSERT(false, "not implemented");
                return Token(TokenType::EOF_Token, "Unterminated string", quoteStartLine, quoteStartColumn);
            }
            value += "\""; 
            advance();
            return Token(TokenType::QuotedIdentifier, value, startLine, startColumn);
        }case '%':{
            std::string value = "%";
            advance();
            
            // Read the hardware type identifier (I, Q, etc.)
            if (!isAtEnd() && std::isalpha(current())) {
                value += current();
                advance();
                
                // Read modifier (X, W, etc.)
                if (!isAtEnd() && std::isalpha(current())) {
                    value += current();
                    advance();
                }
                return Token(TokenType::HardwareType, value, startLine, startColumn);
            }
                
        }
        default: {
            std::string unknown(1, current());
            advance();
            return Token(TokenType::Unknown, unknown, startLine, startColumn);
        }
    }
}

std::vector<Token> Tokenizer::tokenize() {
    if (m_source.empty()) { MIR_ASSERT(false, "Tokenizer source empty!"); }
    

    return tokenizeGeneral();

    
}

} 
}