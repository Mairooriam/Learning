#include "Tokenizer.h"
#include <algorithm>
#include <cctype>

namespace Mir {
namespace Tokenizer{

const std::unordered_set<std::string> Tokenizer::keywords = {
    "type", "struct", "end_struct", "end_type"
};

const std::unordered_set<std::string> Tokenizer::dataTypes = {
    "usint", "bool", "int", "uint", "dint", "udint", "real", "lreal", "string", "time"
    // Add more data types as needed
};

const std::unordered_set<std::string> Tokenizer::boolLiterals = {
    "true", "false"
};

Tokenizer::Tokenizer(const std::string& source) : source(source) {}

char Tokenizer::current() const {
    if (isAtEnd()) return '\0';
    return source[position];
}

char Tokenizer::peek(int offset) const {
    if (position + offset >= source.length()) return '\0';
    return source[position + offset];
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
    return position >= source.length();
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

Token Tokenizer::scanComment() {
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
        }
        default: {
            std::string unknown(1, current());
            advance();
            return Token(TokenType::Unknown, unknown, startLine, startColumn);
        }
    }
}

std::vector<Token> Tokenizer::tokenize() {
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

} 
}