#include "Mapper.h"

namespace Mir
{
    Mapper::Mapper(const json& config, const Types::CSV::Data& data): m_data(data) {
        if (isValidConfig(config))
        {
            m_config = config;
        }else{
            MIR_ASSERT(false, "Not valid confiG. not implemented erorr handling yeat");
        }
        
    }

    Mapper::~Mapper() {}

    // const std::string& templateStr,          // for example                         {location}_{card}_randomtext
    // const std::vector<std::string>& headers, // csv headers "available templates"   { "location", "card"        etc. }
    // const std::vector<std::string>& row   // the actual row to process.             { "CC1"     , "Analoginput" etc. }
    // "typComment": "Generated with Mir",
    // "structName": "{location}_{type}_{suffix1}_typ",
    // "structComment": "{suffix2}",
    // "memberComment": "{card}.{type}",
    // "memberName": "{name}",
    // "memberType": "{type}"
    Types::StructDefinition Mapper::process() {
        
        Types::StructDefinition sDef;
        for (size_t i = 0; i < m_data.content.size(); i++)
        {

            Types::MemberDefinition mDef;
            std::string CommentTemplate =m_config["TypConfigs"]["memberComment"];
            mDef.comment = processTemplate(CommentTemplate,m_data.header,m_data.content[i]);
            
            std::string NameTemplate = m_config["TypConfigs"]["memberName"];
            mDef.name = processTemplate(NameTemplate,m_data.header,m_data.content[i]);

            std::string TypeTempalate =m_config["TypConfigs"]["memberType"];
            mDef.type = processTemplate(TypeTempalate,m_data.header,m_data.content[i]);



            sDef.members.push_back(mDef);
            
        }
        sDef.comment = "Generated with mapper";
        sDef.name = "Gay";
     
        return sDef;
    }

    bool Mapper::isValidConfig(const json& config) {
        if (!config.contains("csvHeader")) {
            return false;
        }
        if (!config.contains("TypConfigs")) {
            return false;
        }
        // Check if TypConfigs contains all required fields
        if (!config["TypConfigs"].contains("typComment")) {
            return false;
        }
        if (!config["TypConfigs"].contains("structName")) {
            return false;
        }
        if (!config["TypConfigs"].contains("structComment")) {
            return false;
        }
        if (!config["TypConfigs"].contains("memberComment")) {
            return false;
        }
        if (!config["TypConfigs"].contains("memberName")) {
            return false;
        }
        if (!config["TypConfigs"].contains("memberType")) {
            return false;
        }
        if (!config.contains("ioCardDataTypes")) {
            return false;
        }
        if (!config.contains("Formatting")) {
            return false;
        }
        if (!config.contains("ioCardAbbreviations")) {
            return false;
        }
        
        return true;
    }

    // input -> AnalogInput01
    std::unordered_map<std::string, std::string> Mapper::processCard(const std::string& card) {
        //"card": "{cardType}{cardDataType}{cardIndex}"
        std::unordered_map<std::string, std::string> result;
        auto [cardType, cardIndex] = Utils::Text::splitCharsFromNums(card);
        Utils::Text::toLowerCase(cardType);
        result["cardType"] = cardType;
        result["cardTypeShort"] = m_config["ioCardAbbreviations"][cardType];
        result["cardIndex"] = cardIndex;
        result["cardDataType"] = m_config["ioCardDataTypes"][cardType];

        return result;
    }

    std::string Mapper::processTemplate(const std::string& templateStr, const std::vector<std::string>& headers,
                                        const std::vector<std::string>& row) {
        std::string result = templateStr;

        std::unordered_map<std::string, size_t> headerMap;
        for (size_t i = 0; i < headers.size(); ++i) {
            headerMap[headers[i]] = i;
        }
        
        size_t startPos = 0;
        while ((startPos = result.find('{', startPos)) != std::string::npos) {
            size_t endPos = result.find('}', startPos);
            if (endPos == std::string::npos) break;
            
            std::string placeholder = result.substr(startPos + 1, endPos - startPos - 1);
            std::string fieldName = placeholder;
            std::string formatName = "default"; 
            
            size_t colonPos = placeholder.find(':');
            if (colonPos != std::string::npos) {
                fieldName = placeholder.substr(0, colonPos);
                formatName = placeholder.substr(colonPos + 1);
            }
            
            if (Utils::mapContains(headerMap, fieldName)) {
                size_t columnIndex = headerMap[fieldName];
                if (columnIndex < row.size()) {
                    std::string fullPlaceholder = "{" + placeholder + "}";
                    
                    if (fieldName == "type") {
                        auto cardInfo = processCard(row[columnIndex]);
                        
                        std::string cardTemplate;
                        if (m_config["placeholders"]["Formatting"]["type"].contains(formatName)) {
                            cardTemplate = m_config["placeholders"]["Formatting"]["type"][formatName];
                        } else {
                            cardTemplate = m_config["placeholders"]["Formatting"]["type"]["default"];
                        }
                        
                        Utils::Text::replacePlaceholderIf(cardTemplate, "cardType", cardInfo["cardType"]);
                        Utils::Text::replacePlaceholderIf(cardTemplate, "cardDataType", cardInfo["cardDataType"]);
                        Utils::Text::replacePlaceholderIf(cardTemplate, "cardIndex", cardInfo["cardIndex"]);
                        Utils::Text::replacePlaceholderIf(cardTemplate, "cardTypeShort", cardInfo["cardTypeShort"]);
                        
                        result.replace(startPos, fullPlaceholder.length(), cardTemplate);
                        startPos += cardTemplate.length();
                    } else {
                        result.replace(startPos, fullPlaceholder.length(), row[columnIndex]);
                        startPos += row[columnIndex].length();
                    }
                } else {
                    startPos = endPos + 1;
                }
            } else {
                MIR_ASSERT(false, "Not handling this case yet. Placeholder not found from header")
                startPos = endPos + 1;
            }
        }
        
        return result;
    }
    

} // namespace Mir
