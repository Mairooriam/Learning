#pragma once
#include "Tokenizer.h"
#include <vector>
#include <string>
namespace Mir {

    


    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // ABSTART SYNTAX TREE .TYP START
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    struct MemberDefinition {
        std::string name;
        std::string type;
        std::string value;
        std::string comment;
    };
    
    struct StructDefinition {
        std::string name;
        std::string comment;
        std::vector<MemberDefinition> members;  
    };
    
    struct TypeDefinition {
        std::string comment;
        std::vector<StructDefinition> structs;  
    };
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // ABSTART SYNTAX TREE .TYP END
    //--------------------------------------------------------------------------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // MIR PARSER START
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // Parsing result wrapper
    namespace Parser{
        struct ParseResult {
            std::vector<TypeDefinition> typDefinitions;
            // Add other format-specific results as needed
        };
        enum class FileFormat{
            UNKOWN,
            TYP,
            IOM
        };


        class MirParser {
        private:
            std::vector<Tokenizer::Token> m_Tokens;
            size_t m_CurrentPosition = 0;
            FileFormat m_FileFormat = FileFormat::UNKOWN;
            // Helper methods
            bool isAtEnd() const { return m_CurrentPosition >= m_Tokens.size()|| 
                m_Tokens[m_CurrentPosition].type ==  Tokenizer::TokenType::EOF_Token; }

                Tokenizer::Token peek() const;
                Tokenizer::Token peekAhead(int n) const;
                Tokenizer::Token advance();
                bool match(Tokenizer::TokenType type);
                void skipUntil(Tokenizer::TokenType type, bool skipMatchedToken = true);
                
                // Format-specific parsing methods
                MemberDefinition parseTypMember();
                StructDefinition parseTypStruct();
                TypeDefinition parseTypTypeBlock();
                std::vector<TypeDefinition> parseTypFile();
                
                // Add IOM-specific parsing methods as needed
                // void parseIomFile();
                
        public:
            MirParser(const std::vector<Tokenizer::Token>& tokens);
            ~MirParser() = default;
            void SetTokens(std::vector<Tokenizer::Token> _Tokens) { m_Tokens = _Tokens; }
            
            // Parse based on file extension
            ParseResult parse();
        };
    }
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // MIR PARSER END
    //--------------------------------------------------------------------------------------------------------------------------------------------------

    
    

    
}