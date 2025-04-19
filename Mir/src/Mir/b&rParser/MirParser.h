#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include "Tokenizer.h"
#include "MirTypes.h"

namespace Mir {


    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // MIR PARSER START
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // Parsing result wrapper
    namespace Parser {
        struct ParseResult {
            std::vector<TypeDefinition> typDefinitions;
            std::vector<VarConfigDefinition> varConfigDefinitions;
        };
        enum class FileFormat { UNKOWN, TYP, IOM };

        class MirParser {
        private:
            std::vector<Tokenizer::Token> m_Tokens;
            size_t m_CurrentPosition = 0;
            FileFormat m_FileFormat = FileFormat::UNKOWN;
            // Helper methods
            bool isEnd() const {
                return m_CurrentPosition >= m_Tokens.size() ||
                       m_Tokens[m_CurrentPosition].type == Tokenizer::TokenType::EOF_Token;
            }

            Tokenizer::Token peek(int offset = 1) const;
            Tokenizer::Token current() const {
                return m_Tokens[m_CurrentPosition];
            }
            Tokenizer::Token advance();
            bool match(Tokenizer::TokenType type);
            void skipUntil(Tokenizer::TokenType type, bool skipMatchedToken = true);

            // .Typ file parsing
            MemberDefinition parseTypMember();
            StructDefinition parseTypStruct();
            TypeDefinition parseTypTypeBlock();
            std::vector<TypeDefinition> parseTypFile();

            // .iom parsing
            VarConfigMemberDefinition parseVarConfigpMember();
            std::vector<VarConfigDefinition> parseIomFile();

            bool expectAndConsume(Tokenizer::TokenType type, const std::string& errorMessage);
            bool expectAndConsume(Tokenizer::TokenType type, std::string& valueOut, const std::string& errorMessage);
            bool expectAndConsumeValue(Tokenizer::TokenType type, const std::string& expectedValue,
                                       const std::string& errorMessage);
            void reportError(const std::string& message);

        public:
            MirParser(const std::vector<Tokenizer::Token>& tokens);
            ~MirParser() = default;
            void SetTokens(std::vector<Tokenizer::Token> _Tokens) {
                m_Tokens = _Tokens;
            }

            // Parse based on file extension
            ParseResult parse();
        };
    }  // namespace Parser
}  // namespace Mir
//--------------------------------------------------------------------------------------------------------------------------------------------------
// MIR PARSER END
//--------------------------------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------------------------------------
// CSV PARSER START
//--------------------------------------------------------------------------------------------------------------------------------------------------
namespace Mir {
    namespace CSV {

        class Parser {
        private:
            Settings m_Settings;
            Data m_data;
        public:
            Parser(Settings _settings);
            ~Parser();
            Data parse();

        };


    }  // namespace CSV
}  // namespace Mir
//--------------------------------------------------------------------------------------------------------------------------------------------------
// CSV PARSER END
//--------------------------------------------------------------------------------------------------------------------------------------------------









