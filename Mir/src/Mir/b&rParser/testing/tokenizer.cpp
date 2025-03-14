#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iomanip>
enum class TokenType {
    Comment,
    Keyword,
    DataType,
    Identifier,
    Operator,
    Semicolon,        // Special type for semicolons (;)
    Comma,            // Special type for commas (,)
    ParenOpen,        // Special type for opening parenthesis (()
    ParenClose,       // Special type for closing parenthesis ())
    Punctuation,
    Literal,
    Whitespace,
    Unknown
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
    
    // Update the toString method in Token struct to include the new token types
    std::string toString() const {
        static const std::unordered_map<TokenType, std::string> typeNames = {
            {TokenType::Comment, "Comment"},
            {TokenType::Keyword, "Keyword"},
            {TokenType::DataType, "DataType"},
            {TokenType::Identifier, "Identifier"},
            {TokenType::Operator, "Operator"},
            {TokenType::Semicolon, "Semicolon"},
            {TokenType::Comma, "Comma"},
            {TokenType::ParenOpen, "ParenOpen"},
            {TokenType::ParenClose, "ParenClose"},
            {TokenType::Literal, "Literal"},
            {TokenType::Whitespace, "Whitespace"},
            {TokenType::Unknown, "Unknown"}
        };
        
        return typeNames.at(type) + ": '" + value + "' at Line " + std::to_string(line) + ", Column " + std::to_string(column);
    }
};

class BRTypTokenizer {
private:
    const std::unordered_map<std::string, TokenType> keywords = {
        {"TYPE", TokenType::Keyword},
        {"END_TYPE", TokenType::Keyword},
        {"STRUCT", TokenType::Keyword},
        {"END_STRUCT", TokenType::Keyword}
    };
    
    const std::unordered_map<std::string, TokenType> dataTypes = {
        {"USINT", TokenType::DataType},
        {"BOOL", TokenType::DataType}
    };
    
    const std::unordered_map<std::string, TokenType> literals = {
        {"TRUE", TokenType::Literal},
        {"FALSE", TokenType::Literal}
    };

public:
    std::vector<Token> tokenize(const std::string& input) {
        std::vector<Token> tokens;
        
        // Define regex for different token types
        std::regex tokenRegex(
            R"((\(\*[\s\S]*?\*\)))"                       // Comments
            R"(|(TYPE|END_TYPE|STRUCT|END_STRUCT))"        // Keywords
            R"(|(USINT|BOOL))"                             // Data Types
            R"(|(:=|:))"                                  // Operators
            R"(|(;))"                                     // Semicolon
            R"(|(,))"                                     // Comma
            R"(|(\())"                                    // Opening parenthesis
            R"(|(\)))"                                    // Closing parenthesis
            R"(|([0-9]+))"                                // Numbers
            R"(|([a-zA-Z_][a-zA-Z0-9_]*))"                // Identifiers
            R"(|(\s+))"                                    // Whitespace
        );
        
        std::sregex_iterator it(input.begin(), input.end(), tokenRegex);
        std::sregex_iterator end;
        
        int lineNumber = 1;
        int columnNumber = 1;
        
        for (; it != end; ++it) {
            std::smatch match = *it;
            std::string value = match.str();
            TokenType type = TokenType::Unknown;
            
            // Use a case-like approach to determine token type based on matched groups
            if (match[1].matched)       type = TokenType::Comment;
            else if (match[2].matched)  type = TokenType::Keyword;
            else if (match[3].matched)  type = TokenType::DataType;
            else if (match[4].matched)  type = TokenType::Operator;
            else if (match[5].matched)  type = TokenType::Semicolon;
            else if (match[6].matched)  type = TokenType::Comma;
            else if (match[7].matched)  type = TokenType::ParenOpen;
            else if (match[8].matched)  type = TokenType::ParenClose;
            else if (match[9].matched)  type = TokenType::Literal; // Number literals
            else if (match[10].matched) {
                // Check if it's a keyword, datatype, or literal first
                std::string upperValue = value;
                for (char& c : upperValue) {
                    c = std::toupper(c);
                }
                
                if (keywords.find(upperValue) != keywords.end()) {
                    type = keywords.at(upperValue);
                } else if (dataTypes.find(upperValue) != dataTypes.end()) {
                    type = dataTypes.at(upperValue);
                } else if (literals.find(upperValue) != literals.end()) {
                    type = literals.at(upperValue);
                } else {
                    type = TokenType::Identifier;
                }
            } 
            else if (match[11].matched) type = TokenType::Whitespace;
            // Only add non-whitespace tokens since whitespace doesn't matter
            if (type != TokenType::Whitespace) {
                tokens.push_back({type, value, lineNumber, columnNumber});
            }
            
            // Update line and column numbers
            for (char c : value) {
                if (c == '\n') {
                    lineNumber++;
                    columnNumber = 1;
                } else {
                    columnNumber++;
                }
            }
        }
        
        return tokens;
    }
};

int main() {
    BRTypTokenizer tokenizer;
    
    // Read the file content
    std::ifstream file("TokenizerTestData.typ");
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }
    
    std::string content((std::istreambuf_iterator<char>(file)), 
                        std::istreambuf_iterator<char>());
    file.close();
    
    std::vector<Token> tokens = tokenizer.tokenize(content);
    
    // Print tokens using fixed-width formatting
    std::cout << std::left
              << std::setw(15) << "Type"
              << std::setw(25) << "Value"
              << std::setw(8) << "Line"
              << std::setw(8) << "Column" << std::endl;
    std::cout << std::string(56, '-') << std::endl;
    
    for (const auto& token : tokens) {
        std::string typeStr;
        switch (token.type) {
            case TokenType::Comment: typeStr = "Comment"; break;
            case TokenType::Keyword: typeStr = "Keyword"; break;
            case TokenType::DataType: typeStr = "DataType"; break;
            case TokenType::Identifier: typeStr = "Identifier"; break;
            case TokenType::Operator: typeStr = "Operator"; break;
            case TokenType::Semicolon: typeStr = "Semicolon"; break;
            case TokenType::Comma: typeStr = "Comma"; break;
            case TokenType::ParenOpen: typeStr = "ParenOpen"; break;
            case TokenType::ParenClose: typeStr = "ParenClose"; break;
            case TokenType::Literal: typeStr = "Literal"; break;
            case TokenType::Unknown: typeStr = "Unknown"; break;
            default: typeStr = "Other"; break;
        }
        
        // Truncate long values (especially comments) for display
        std::string displayValue = token.value;
        if (displayValue.length() > 20) {
            displayValue = displayValue.substr(0, 17) + "...";
        }
        
        // Handle newlines in display value
        size_t pos = displayValue.find('\n');
        while (pos != std::string::npos) {
            displayValue.replace(pos, 1, "\\n");
            pos = displayValue.find('\n', pos + 2);
        }
        
        // Print with fixed-width formatting
        std::cout << std::left
                  << std::setw(15) << typeStr
                  << std::setw(25) << displayValue
                  << std::setw(8) << token.line
                  << std::setw(8) << token.column << std::endl;
    }
    
    std::cout << std::string(56, '-') << std::endl;
    std::cout << "Total tokens: " << tokens.size() << std::endl;
    
    // Print token statistics
    std::unordered_map<TokenType, int> typeCounts;
    for (const auto& token : tokens) {
        typeCounts[token.type]++;
    }
    
    std::cout << "\nToken Statistics:" << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << "Comments:       " << typeCounts[TokenType::Comment] << std::endl;
    std::cout << "Keywords:       " << typeCounts[TokenType::Keyword] << std::endl;
    std::cout << "Data Types:     " << typeCounts[TokenType::DataType] << std::endl;
    std::cout << "Identifiers:    " << typeCounts[TokenType::Identifier] << std::endl;
    std::cout << "Operators:      " << typeCounts[TokenType::Operator] << std::endl;
    std::cout << "Semicolons:     " << typeCounts[TokenType::Semicolon] << std::endl;
    std::cout << "Commas:         " << typeCounts[TokenType::Comma] << std::endl;
    std::cout << "Open Parens:    " << typeCounts[TokenType::ParenOpen] << std::endl;
    std::cout << "Close Parens:   " << typeCounts[TokenType::ParenClose] << std::endl;
    std::cout << "Literals:       " << typeCounts[TokenType::Literal] << std::endl;
    std::cout << "Unknown:        " << typeCounts[TokenType::Unknown] << std::endl;
    
    return 0;
}