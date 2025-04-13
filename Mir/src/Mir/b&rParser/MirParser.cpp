#include "MirParser.h"
#include "Core.h"
namespace Mir {
    namespace Parser{


    // void MirParser::parseMirCsv()
    // {
    //     TypeDefinition typdef;
    //     StructDefinition structdef;
    //     MemberDefinition memberdef;
    //     std::vector<std::string> headerbuf;
        

    // }

    // // bool MirParser::validHeaderAtColumn(const std::string& headerValue, size_t columnIndex)
    // // {
    // //     const std::vector<std::string> expectedHeaders = {
    // //     "Location", "Type", "BR Name", "Card", "Eplan name"
    // //     };
        
    // //     if (columnIndex >= expectedHeaders.size()) {
    // //     MIR_ASSERT(false, "Invalid column index: " + std::to_string(columnIndex) + 
    // //             " exceeds expected header count " + std::to_string(expectedHeaders.size()));
    // //     return false;
    // //     }
        
    // //     if (headerValue != expectedHeaders[columnIndex]) {
    // //     MIR_ASSERT(false, "Invalid header at column " + std::to_string(columnIndex) + 
    // //             ": Expected '" + expectedHeaders[columnIndex] + "', got '" + headerValue + "'");
    // //     return false;
    // //     }
        
    // //     return true;
    // // }

    // // bool MirParser::isValidHeader(const std::vector<std::string> &header)
    // // {
    // //     const std::vector<std::string> expectedHeaders = {
    // //     "Location", "Type", "BR Name", "Card", "Eplan name"
    // //     };
        
    // //     if (header.size() != expectedHeaders.size()) {
    // //     MIR_ASSERT(false,"Invalid header: Expected " + std::to_string(expectedHeaders.size()) + 
    // //             " fields, got " + std::to_string(header.size()));
    // //     return false;
    // //     }
    
    // //     for (size_t i = 0; i < header.size(); ++i) {
    // //     if (!validHeaderAtColumn(header[i], i)) {
    // //         return false;
    // //     }
    // //     }
        
    // //     return true;
    // // }

    bool MirParser::expectAndConsume(Tokenizer::TokenType type, const std::string &errorMessage)
    {
        if (current().type != type) {
            reportError(errorMessage);
            return false;
        }
        advance();
        return true;
    }

    bool MirParser::expectAndConsume(Tokenizer::TokenType type, std::string &valueOut, const std::string &errorMessage)
    {
        if (current().type != type) {
            reportError(errorMessage);
            return false;
        }
        valueOut = current().value;
        advance();
        return true;

    }

    bool MirParser::expectAndConsumeValue(Tokenizer::TokenType type, const std::string &expectedValue, const std::string &errorMessage)
    {
        if (current().type != type || current().value != expectedValue) {
            reportError(errorMessage);
            return false;
        }
        advance();
        return true;
    }

    void MirParser::reportError(const std::string &message)
    {
        // Replace assertion with better error reporting
        std::string errorMsg = message + " at line " + 
                                std::to_string(current().line) + ", column " + 
                                std::to_string(current().column);
        MIR_CORE_ERROR(errorMsg);

    }
    
    MirParser::MirParser(const std::vector<Tokenizer::Token> &tokens)
        : m_Tokens(tokens)
    {

        skipUntil(Tokenizer::TokenType::Keyword, false);
        std::string value = m_Tokens[m_CurrentPosition].value;
        if (value == "TYPE")
        {
            m_FileFormat = FileFormat::TYP;
        }
        else if (value == "VAR_CONFIG")
        {
            m_FileFormat = FileFormat::IOM;
        }
        else
        {
            MIR_ASSERT(false, "NOT IMPLEMENTED UNKOWN")
        }
        m_CurrentPosition = 0;
    }

    ParseResult MirParser::parse()
    {
        ParseResult result;
        if (m_FileFormat == FileFormat::TYP)
        {
            result.typDefinitions = parseTypFile();
        }

        if (m_FileFormat == FileFormat::IOM)
        {
            result.varConfigDefinitions = parseIomFile();
        }
        return result;
    }

    Tokenizer::Token MirParser::peek(int offset) const {
        if (m_CurrentPosition + offset >= m_Tokens.size()) {
            return Tokenizer::Token(Tokenizer::TokenType::EOF_Token, "", 0, 0);
        }
        return m_Tokens[m_CurrentPosition + offset];
    }

    Tokenizer::Token MirParser::advance()
    {
        if (!isEnd()) {
            m_CurrentPosition++;
        }
        return m_Tokens[m_CurrentPosition - 1];
    }
    bool MirParser::match(Tokenizer::TokenType type) {
        if (isEnd()) return false;
        if (current().type != type) return false;
        
        advance();
        return true;
    }

    void MirParser::skipUntil(Tokenizer::TokenType type, bool skipMatchedToken) {
        while (!isEnd() && current().type != type) {
            advance();
        }
        
        // Skip the matched token if requested
        if (!isEnd() && skipMatchedToken) {
            advance();
        }
    }

    MemberDefinition MirParser::parseTypMember()
    {
        MemberDefinition result;
        
        // We expect to be at an identifier (member name)
        if (current().type == Tokenizer::TokenType::Identifier) {
            result.name = current().value;
            advance();
            
            // Check for colon
            if (match(Tokenizer::TokenType::Colon)) {
                // Parse data type
                if (current().type == Tokenizer::TokenType::DataType || 
                    current().type == Tokenizer::TokenType::Identifier) {
                    result.type = current().value;
                    advance();
                }
                
                // Check for assignment
                if (match(Tokenizer::TokenType::Assignment)) {
                    // Parse value
                    if (current().type == Tokenizer::TokenType::BoolLiteral) {
                        result.value = current().value;
                        advance();
                    }
                }
            }

            // Check for comment
            if (match(Tokenizer::TokenType::Semicolon)) {
                
                if (current().type == Tokenizer::TokenType::Comment) {
                    result.comment = current().value;
                    advance();
                }
            }

            
            // Skip semicolon

        }
        
        return result;
    }

    StructDefinition MirParser::parseTypStruct()
    {
        StructDefinition result;
        
        // We expect to be at an identifier (struct name)
        if (current().type == Tokenizer::TokenType::Identifier) {
            result.name = current().value;
            advance();
            
            // Check for colon
            if (match(Tokenizer::TokenType::Colon)) {
                // Look for STRUCT keyword
                if (current().type == Tokenizer::TokenType::Keyword && 
                    current().keywordType == Tokenizer::Keyword::STRUCT) {
                    
                    advance(); // Skip STRUCT keyword
                    
                    // Check for comment after STRUCT
                    if (current().type == Tokenizer::TokenType::Comment) {
                        result.comment = current().value;
                        advance();
                    }
                    
                    // Parse members until END_STRUCT
                    while (!isEnd() && 
                           !(current().type == Tokenizer::TokenType::Keyword && 
                             current().keywordType == Tokenizer::Keyword::END_STRUCT)) {
                        
                        if (current().type == Tokenizer::TokenType::Identifier) {
                            MemberDefinition member = parseTypMember();
                            result.members.push_back(member);
                        } else {
                            advance(); // Skip unexpected tokens
                        }
                    }
                    
                    // Skip END_STRUCT
                    if (current().type == Tokenizer::TokenType::Keyword && 
                        current().keywordType == Tokenizer::Keyword::END_STRUCT) {
                        advance();
                        
                        // Skip optional semicolon after END_STRUCT
                        if (current().type == Tokenizer::TokenType::Semicolon) {
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
        
        while (!isEnd()) {
            TypeDefinition typeDefinition;
            
            // Look for comments before TYPE
            if (current().type == Tokenizer::TokenType::Comment) {
                typeDefinition.comment = current().value;
                advance();
            }
            
            // Look for TYPE keyword
            if (current().type == Tokenizer::TokenType::Keyword && 
                current().keywordType == Tokenizer::Keyword::TYPE) {
                
                advance(); // Skip TYPE keyword
                
                // Parse structs until END_TYPE
                while (!isEnd() && 
                      !(current().type == Tokenizer::TokenType::Keyword && 
                        current().keywordType == Tokenizer::Keyword::END_TYPE)) {
                    
                    // Check for struct definition
                    if (current().type == Tokenizer::TokenType::Identifier) {
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
                if (current().type == Tokenizer::TokenType::Keyword && 
                    current().keywordType == Tokenizer::Keyword::END_TYPE) {
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
    VarConfigMemberDefinition MirParser::parseVarConfigpMember()
    {
        VarConfigMemberDefinition memberDef;

        std::string processVar;
    
        if (!expectAndConsume(Tokenizer::TokenType::DoubleColon, processVar, "Expected :: at start of process variable")) {
            return memberDef;
        }
        
        std::string varName;
        if (!expectAndConsume(Tokenizer::TokenType::Identifier, varName, "Expected identifier after ::")) {
            return memberDef;
        }
        processVar += varName;
        
        std::string dot, memberName;
        if (match(Tokenizer::TokenType::Dot)) {
            processVar += "."; // We consumed the dot with match()
            
            if (!expectAndConsume(Tokenizer::TokenType::Identifier, memberName, "Expected member name after dot")) {
                return memberDef;
            }
            processVar += memberName;
        }
        
        memberDef.processVar = processVar;
        
        if (!expectAndConsumeValue(Tokenizer::TokenType::Keyword, "AT", "Expected AT keyword after process variable")) {
            return memberDef;
        }
        
        if (!expectAndConsume(Tokenizer::TokenType::HardwareType, memberDef.hardwareType, "Expected hardware type after AT")) {
            return memberDef;
        }
        
        if (!expectAndConsume(Tokenizer::TokenType::Dot, "Expected dot after hardware type")) {
            return memberDef;
        }
        
        std::string ioAddress;
        std::string cardName;
        if (!expectAndConsume(Tokenizer::TokenType::QuotedIdentifier, cardName, "Expected quoted card name")) {
            return memberDef;
        }
        ioAddress += cardName;
        
        if (!expectAndConsume(Tokenizer::TokenType::Dot, dot, "Expected dot after card name")) {
            return memberDef;
        }
        ioAddress += dot;
        
        std::string channelName;
        if (!expectAndConsume(Tokenizer::TokenType::Identifier, channelName, "Expected channel identifier")) {
            return memberDef;
        }
        ioAddress += channelName;
        
        memberDef.ioAdress = ioAddress;
        
        if (!expectAndConsume(Tokenizer::TokenType::Semicolon, "Expected semicolon after I/O address")) {
            return memberDef;
        }
        
        if (current().type == Tokenizer::TokenType::Comment) {
            memberDef.comment = current().value;
            advance();
        }
        

        return memberDef;
    }
    std::vector<VarConfigDefinition> MirParser::parseIomFile()
    {
        std::vector<VarConfigDefinition> result;
        m_CurrentPosition = 0;
        
        while (!isEnd()) {
            VarConfigDefinition varConfigDefinition;
            
            if (current().type == Tokenizer::TokenType::Comment) {
                varConfigDefinition.comment = current().value;
                advance();
            }
            
            if (current().type == Tokenizer::TokenType::Keyword && 
                current().keywordType == Tokenizer::Keyword::VAR_CONFIG) {
                advance(); 
        
                // while not VAR_END
                while (!isEnd() && 
                      !(current().type == Tokenizer::TokenType::Keyword && 
                        current().keywordType == Tokenizer::Keyword::END_VAR)) {
                    
                    // Check for ::
                    if (current().type == Tokenizer::TokenType::DoubleColon) {
                        VarConfigMemberDefinition varConfigDef = parseVarConfigpMember();
                        if (!varConfigDef.ioAdress.empty()) {
                            varConfigDefinition.members.push_back(varConfigDef);
                        }
                    } else {
                        // Skip anything that's not an identifier
                        advance();
                    }
                }
                
                // Skip END_VAR
                if (current().type == Tokenizer::TokenType::Keyword && 
                    current().keywordType == Tokenizer::Keyword::END_VAR) {
                    advance();
                }
                
                // Add type definition to results
                if (!varConfigDefinition.members.empty()) {
                    result.push_back(varConfigDefinition);
                }
            } else {
                // Skip tokens until we find VAR CONFIG or EOF
                advance();
            }
        }
        
        return result;
    }

    }  // namespace Parser
    namespace CSV {
        Parser::Parser(Settings _settings) {
            if (!Utils::File::hasExtension(_settings.targetFile, ".csv"))
            {
                MIR_ASSERT(false, "CSV::Parser was intialized with correct file format");
            } else{
                m_Settings = _settings;
            }
            

        }
        Parser::~Parser() {}


        
        Data Parser::parse() {
            std::ifstream file = Utils::File::openFile(m_Settings.targetFile);
            std::string line;
        
            if (std::getline(file, line)) {
                m_data.header = Utils::Text::splitLine(line, ',');
            }
        
            // Read data lines
            while (std::getline(file, line)) {
                m_data.content.push_back(Utils::Text::splitLine(line, ','));
            }
            
            return m_data;
        }
    }  // namespace CSV

} // namespace Mir


