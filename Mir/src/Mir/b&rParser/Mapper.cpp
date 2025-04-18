#include "Mapper.h"

namespace Mir
{
    Mapper::Mapper(const json& config) {
        if (isValidConfig(config))
        {
            m_config = config;
        }else{
            MIR_ASSERT(false, "Not valid confiG. not implemented erorr handling yeat");
        }
        
    }

    Mapper::~Mapper() {}

    std::string Mapper::process(CSV::Data data) {
        for (size_t i = 0; i < data.content.size(); i++)
        {
            processTemplate(,,data.content[i]);
        }
        
     
        return std::string();
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
            
            std::string fieldName = result.substr(startPos + 1, endPos - startPos - 1);
           

            // if fieldname is valid ->> replace {location} -> CC1 etc.
            if (Utils::mapContains(headerMap, fieldName)) {
                size_t columnIndex = headerMap[fieldName];
                if (columnIndex < row.size()) {
                    std::string placeholder = "{" + fieldName + "}";
                    
                    // If formatting {card}
                    // Formatting card: {name}{number}{cardDataType}
                    if (fieldName == "card")
                    {

                        auto test22 =  processCard(row[columnIndex]);
                        std::string cardTemplate = m_config["Formatting"]["card"];
                        Utils::Text::replacePlaceholderIf(cardTemplate, "cardType", test22["cardType"]);
                        Utils::Text::replacePlaceholderIf(cardTemplate, "cardDataType", test22["cardDataType"]);
                        Utils::Text::replacePlaceholderIf(cardTemplate, "cardIndex", test22["cardIndex"]);
                        result.replace(startPos, placeholder.length(), cardTemplate);

                    }else {
                        result.replace(startPos, placeholder.length(), row[columnIndex]);
                    }
                    

                    
                    
                    startPos += row[columnIndex].length();
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
