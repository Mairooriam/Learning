#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include "Tokenizer.h"
#include "Types/MirTypes.h"


namespace Mir {


    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // MIR PARSER START
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // Parsing result wrapper
    namespace Parser {
        struct ParseResult {
            std::vector<Types::TypeDefinition> typDefinitions;
            std::vector<Types::VarConfigDefinition> varConfigDefinitions;
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
            Types::MemberDefinition parseTypMember();
            Types::StructDefinition parseTypStruct();
            Types::TypeDefinition parseTypTypeBlock();
            std::vector<Types::TypeDefinition> parseTypFile();

            // .iom parsing
            Types::VarConfigMemberDefinition parseVarConfigpMember();
            std::vector<Types::VarConfigDefinition> parseIomFile();

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

    class CsvParser {
        private:
            Types::CSV::Settings m_Settings;
            Types::CSV::Data m_data;
        public:
            CsvParser(Types::CSV::Settings _settings);
            ~CsvParser();
            Types::CSV::Data parse();

        };

    }  // namespace Parser
}  // namespace Mir
//--------------------------------------------------------------------------------------------------------------------------------------------------
// MIR PARSER END
//--------------------------------------------------------------------------------------------------------------------------------------------------






