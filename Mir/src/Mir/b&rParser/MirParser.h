#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include "Tokenizer.h"
#include "Utils/Utils.h"

namespace Mir {

    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // ABSTART SYNTAX TREE  START
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // TYP
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
    // IOM
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    struct VarConfigMemberDefinition {
        std::string processVar;  // The process variable components
        std::string ioAdress;
        std::string hardwareType;  // Data type (e.g., "%IW")
        std::string comment;       // Optional comment (e.g., "Generated with Mir")
    };

    struct VarConfigDefinition {
        std::string comment;
        std::vector<VarConfigMemberDefinition> members;
    };
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // ABSTART SYNTAX TREE END
    //--------------------------------------------------------------------------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // AST UTILS START
    //--------------------------------------------------------------------------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // AST UTILS END
    //--------------------------------------------------------------------------------------------------------------------------------------------------

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
        struct Settings {
            std::filesystem::path targetFile;
            std::string header;
            char delimeter;
        };
        struct Data {
            std::vector<std::string> header;
            std::vector<std::vector<std::string>> content;
        };

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
