#include <fstream>
#include <iostream>
#include <string>
#include <map>

#include "parser.h"

namespace Mir {
    
    std::vector<std::string> brParser::splitStringStruct(const std::string &str) {
        std::vector<std::string> result(4); // Initialize with 4 empty strings
        
        // Find the first colon to split name and rest
        size_t colonPos = str.find(':');
        if (colonPos != std::string::npos) {
            // Extract name (token[0])
            result[0] = str.substr(0, colonPos);
            
            // Get remaining string after name
            std::string remainder = str.substr(colonPos + 1);
            
            // Look for ":=" to separate type and value
            size_t assignPos = remainder.find(":=");
            if (assignPos != std::string::npos) {
                // Has value - original logic
                result[1] = remainder.substr(0, assignPos);
                
                std::string valueAndComment = remainder.substr(assignPos + 2);
                
                size_t semicolonPos = valueAndComment.find(';');
                if (semicolonPos != std::string::npos) {
                    result[2] = valueAndComment.substr(0, semicolonPos);
                    
                    std::string afterSemicolon = valueAndComment.substr(semicolonPos);
                    size_t commentStart = afterSemicolon.find("(*");
                    if (commentStart != std::string::npos) {
                        size_t commentEnd = afterSemicolon.find("*)");
                        if (commentEnd != std::string::npos) {
                            result[3] = afterSemicolon.substr(commentStart + 2,
                                                            commentEnd - commentStart - 2);
                        }
                    }
                } else {
                    result[2] = valueAndComment;
                }
            } else {
                // No value - look for comment directly after type
                size_t semicolonPos = remainder.find(';');
                if (semicolonPos != std::string::npos) {
                    result[1] = remainder.substr(0, semicolonPos);
                    
                    std::string afterSemicolon = remainder.substr(semicolonPos);
                    size_t commentStart = afterSemicolon.find("(*");
                    if (commentStart != std::string::npos) {
                        size_t commentEnd = afterSemicolon.find("*)");
                        if (commentEnd != std::string::npos) {
                            result[3] = afterSemicolon.substr(commentStart + 2,
                                                            commentEnd - commentStart - 2);
                        }
                    }
                } else {
                    result[1] = remainder;
                }
            }
        }
        
        // Trim whitespace from all tokens
        for (auto& token : result) {
            token.erase(0, token.find_first_not_of(" \t\n\r\f\v"));
            token.erase(token.find_last_not_of(" \t\n\r\f\v") + 1);
        }
        
        return result;
    }

    brVarConfigCollection brParser::readBrVarConfig(const std::string &path)
    {
        brVarConfigCollection configCollection;

        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << path << std::endl;

        }

        std::string                 line;
        std::string                 lastLine;
        std::vector<std::string>    tokens;


        std::stringstream          lineStream(line);
        std::string                cell;
        std::string result;


        while(std::getline(file,line))
        {
            if (line == "VAR_CONFIG")
            {
                brVarConfigData configData;
                
                brVarConfigNode bufferconfigNode;
                


                if (brIsComment(lastLine))
                {
                    bufferconfigNode.comment = lastLine.substr(2, lastLine.length() - 4);  // Remove "(*" and "*)"


                }
                bool endFound = false;
                while (std::getline(file,line))
                {
                    if(line == ""){
                        continue;
                    }

                    if(line == "END_VAR"){
                        configCollection.config.push_back(bufferconfigNode);
                        bufferconfigNode.clear();
                        break;
                    }

                    
                    bool commentStarted = false;
                    std::stringstream ss(line);
                    std::string value;
                    std::string commentBuffer;
        
                    std::vector<std::string> tokens;
                    while (std::getline(ss, value, ' ')) {
                        // Remove leading whitespace
                        value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));
                        
                        if (((value.size() >= 2 && value[0] == '(' && value[1] == '*') || commentStarted)) {
                            commentStarted = true;
                            if (!commentBuffer.empty()) {
                                commentBuffer += " ";
                            }
                            commentBuffer += value;
                            
                            // Check if comment ends in this token
                            if (value.size() >= 2 && 
                                value[value.size()-2] == '*' && 
                                value[value.size()-1] == ')') {
                                commentStarted = false;
                                // Remove leading and trailing comment markers for the final result
                                commentBuffer.erase(0, commentBuffer.find("(*") + 2);
                                commentBuffer.erase(commentBuffer.find("*)"));
                                tokens.push_back(commentBuffer);
                                commentBuffer.clear();
                            }
                            continue; 
                        }
                        tokens.push_back(value);
                    }

            
                    if (tokens[1] == "AT" )
                    {
                        if (!tokens[2].empty()) {
                            size_t dotPos = tokens[2].find('.');
                            if (dotPos != std::string::npos) {
                                configData.type = tokens[2].substr(0, dotPos);   
                            }
                        }
                        configData.ioAdress = tokens[2].substr(4); // Remove %XX. prefix
                        // Extract the I/O type prefix (%IX, %QX, etc.) from the address
                        
                        configData.processVariable = tokens[0];
                        if (tokens.size() <= 3)
                        {
                            configData.comment = "";
                        }else{
                            configData.comment = tokens[3];
                            if (!configData.ioAdress.empty()) {
                                configData.ioAdress.pop_back();
                            }
                        }
                        
                    }
            
                    bufferconfigNode.values.push_back(configData);    
                }    
            }
            lastLine = line;
        }    
        return configCollection;
    }

    void writeDummyData()
    {

    }

    brParser::brParser()
    {
    }

    brParser::~brParser()
    {
    }

    std::string brParser::readFile(const std::string &path)
    {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << path << std::endl;
            return "ERROR";
        }
    
        std::string line;
        std::string content;
        while (std::getline(file, line)) {
            // Process each line of the file
            std::cout << line << std::endl;
            content += line + "\n"; // Add each line to the content string
        }

        file.close();
        return content;
    }



    brTyp brParser::readDatafile999999(const std::string &path)
    {

        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << path << std::endl;
            return brTyp();
        }

        std::string                 line;
        size_t                      LineIndex = 1;
        std::string                 lastLine;
        std::vector<std::string>    tokens;
        bool                        typeFound = 0;

        std::stringstream          lineStream(line);
        std::string                cell;


        brTyp result;
        while(std::getline(file,line))
        {
            LineIndex++;

            if (line == "") continue;




            if (line == "TYPE")
            {   
                brStructCollection col;
                brStructNode node;
                bool structFound1 = false;
                while (std::getline(file,line))
                {
                    removeSpaces(line);

                    if (line == "END_TYPE")
                    {
                        result.push_back(col);
                        break;
                    }
                    if (line== "END_STRUCT;")
                    {
                        col.structs.push_back(node);
                        node.clear();
                        structFound1 = false; 
                        continue;
                    }
                    
                    
                    if (brIsComment(lastLine))
                    {
                        col.comment = lastLine.substr(2, lastLine.length() - 4);  // Remove "(*" and "*)"
                        lastLine = "";
                    }
                    
                    if (!structFound1)
                    {
                        tokens = splitStringStruct(line);
                        if (tokens[1]=="STRUCT")
                        {
                            node.name = tokens[0];
                            structFound1 = true;
                        } 
                    } else{
                        brStructData data;               

                        tokens = splitStringStruct(line);
                        data.name = tokens[0];
                        data.type = tokens[1];
                        data.value = tokens[2];
                        data.comment = tokens[3];  
                        node.values.push_back(data);
                    }
                }
            }
            lastLine = line;
        }
        

        return result;
    }



    std::string brParser::removeSpaces(std::string &str)
    {
        str.erase(std::remove_if(str.begin(), str.end(), 
                     [](char c) { return std::isspace(c); }),
                 str.end());
        return str;
    }
    struct cardInfo {
        std::string shortName;
        std::string fullname;
        std::string number;
        std::string datatype;

        std::string getShortName() {
            if (fullname.find("DigitalInput") != std::string::npos) return "Di";
            if (fullname.find("DigitalOutput") != std::string::npos) return "Do";
            if (fullname.find("AnalogInput") != std::string::npos) return "Ai";
            if (fullname.find("AnalogOutput") != std::string::npos) return "Ao";
            return "Unknown";
        }

        std::string getDataType() {
            if (shortName == "Di") return "BOOL";
            if (shortName == "Do") return "BOOL";
            if (shortName == "Ai") return "REAL";
            if (shortName == "Ao") return "REAL";
            return "UNKNOWN";
        }
        cardInfo(std::string cardName) {
            fullname = cardName;
            if (!cardName.empty()) {
            // Make sure the card name has at least 2 characters for safe substring
            if (cardName.size() >= 2) {
                number = cardName.substr(cardName.size()-2, 2);
            } else {
                number = cardName;
            }
            } else {
            number = "";
            }
            shortName = getShortName();
            datatype = getDataType();
        }
        };


    std::string brParser::sanitizeString(const std::string& input) {
        std::string result;
        result.reserve(input.length());
        
        for (char c : input) {
            if (std::isalnum(c)) {
                result += c;
            } else if (c == ' ' || c == '-') {
                // Replace spaces and hyphens with underscore
                result += '_';
            }
            // Ignore all other special characters
        }
        
        // Remove consecutive underscores
        auto it = std::unique(result.begin(), result.end(), 
            [](char a, char b) { return a == '_' && b == '_'; });
        result.erase(it, result.end());
        
        // Remove trailing underscores
        while (!result.empty() && result.back() == '_') {
            result.pop_back();
        }
        
        return result;
    }



    std::vector<std::vector<std::string>> brParser::readPlcDataCsv(const std::string &path, const std::string& header) {
        std::vector<std::vector<std::string>> csvStr;
        std::ifstream file(path);
        std::string line;
        
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << path << std::endl;
            return csvStr;
        }
        
        if (!std::getline(file, line) || line != header) {
            return csvStr;
        }
        
        while (std::getline(file, line)) {
            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string value;

            while (std::getline(ss, value, ',')) {
                row.push_back(value);
            }

            if (!line.empty() && line.back() == ',') {
                row.push_back("");
            }

            size_t emptyCount = 0;
            for (auto &&el : row) {
                if (el == "") 
                    emptyCount++;  
            }

            if (emptyCount >= 5) {
                MIR_CORE_ASSERT(false, "Add handling for when to not count empty fields?");
            }
            
            if (row.size() == 5) {
                csvStr.push_back(row);
            } else { 
                MIR_CORE_ASSERT(false, "Error in csv string add handling"); 
            }
        }
        
        return csvStr;
    }

    void brParser::writeFile(const std::string &path, const std::string &content, std::ios_base::openmode mode)
    {
        std::ofstream file(path, mode);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << path << std::endl;
            return;
        }

        file << content;

        file.close();
    }
    enum csvNames{
        Location = 0,
        Type,
        Name,
        IoCardName,
        EplanName
    };

    brStructCollection brParser::parseCsvIntoBrCollection(std::vector<std::vector<std::string>> &csvStr)
    {
        brStructCollection result;
        brStructNode structNode;
        
        // Group entries by card type
        std::map<std::string, std::vector<brStructData>> cardGroups;
        
        for (auto &&row : csvStr)
        {
            cardInfo cardinfo(row[Type]);

            std::string key = row[Location] + "_" + cardinfo.shortName + "_typ";
            std::string comment = row[IoCardName] + "." + cardinfo.shortName + "." + cardinfo.number;
            cardGroups[key].push_back(brStructData(sanitizeString(row[Name]),cardinfo.datatype,"",comment));
        }
        
        for (auto &&[key, value] : cardGroups)
        {
            result.comment = "imported stuff";
            brStructNode node;
            node.name = key;
            node.values = value;
            result.structs.push_back(node);
        }
        
        
        return result;
    }

    brVarConfigNode brParser::parseSturctToConfig(const brStructNode &node)
    {
        brVarConfigNode configNode;
        brVarConfigCollection configCol;



        std::string name = node.name.substr(0, node.name.length() - 4);


        std::string comment;
        for (size_t j = 0; j < node.values.size(); j++)
        {
            std::string processVariable = "::" + name + "." + node.values[j].name;
            comment = node.values[j].comment;

            std::string ioAddress;
            std::stringstream ss(comment);
            std::string segment;
            size_t i = 0;
            brVarConfigData configData;
            while (std::getline(ss, segment, '.')) {
                if (i == 0){ ioAddress += "\"" + segment + "\"."; }
                if (i == 2){ ioAddress += segment; } 

                if (i == 1){ 
                    ioAddress += getLongCardType(segment); 
                    configData.type = shortCardTypeToVarConfigType(segment);
                }
                        
                i++;
            }
            
            configData.ioAdress = ioAddress;
            configData.processVariable = processVariable;
            configData.comment = "Generated with Mir";
            configNode.values.push_back(configData);
        }
        configNode.comment = "Generated with Mir";
        return configNode;

    }

    brVarConfigNode brParser::parseCollectionToConfig(const brStructCollection &col)
    {
        brVarConfigNode configNode;
        brVarConfigCollection configCol;
        for (auto &brStruct : col)
        {
            std::string name = brStruct.name.substr(0, brStruct.name.length() - 4);


            std::string comment;
            for (size_t j = 0; j < brStruct.values.size(); j++)
            {
                std::string processVariable = "::" + name + "." + brStruct.values[j].name;
                comment = brStruct.values[j].comment;
    
                std::string ioAddress;
                std::stringstream ss(comment);
                std::string segment;
                size_t i = 0;
                brVarConfigData configData;
                while (std::getline(ss, segment, '.')) {
                    if (i == 0){ ioAddress += "\"" + segment + "\"."; }
                    if (i == 2){ ioAddress += segment; } 
    
                    if (i == 1){ 
                        ioAddress += getLongCardType(segment); 
                        configData.type = shortCardTypeToVarConfigType(segment);
                    }
                            
                    i++;
                }
                
                configData.ioAdress = ioAddress;
                configData.processVariable = processVariable;
                configData.comment = "Generated with Mir";
                configNode.values.push_back(configData);
            }
            configNode.comment = "Generated with Mir";
           
        }
        return configNode;
    }

    std::vector<brVarConfigNode> brParser::parseCollectionToConfigMap(const brStructCollection &col)
    {
        // Map to temporarily organize data by card name
        std::unordered_map<std::string, brVarConfigNode> tempNodeMap;
        
        for (auto &brStruct : col)
        {
            std::string name = brStruct.name.substr(0, brStruct.name.length() - 4);
    
            for (size_t j = 0; j < brStruct.values.size(); j++)
            {
                std::string processVariable = "::" + name + "." + brStruct.values[j].name;
                std::string comment = brStruct.values[j].comment;
    
                std::string ioAddress;
                std::stringstream ss(comment);
                std::string segment;
                size_t i = 0;
                brVarConfigData configData;
                
                // Card name to be extracted
                std::string cardName;
                
                while (std::getline(ss, segment, '.')) {
                    if (i == 0) {
                        // Extract card name (e.g., "af104")
                        cardName = segment;
                        // Remove quotes if present
                        if (cardName.front() == '"' && cardName.back() == '"') {
                            cardName = cardName.substr(1, cardName.length() - 2);
                        }
                        // Convert to uppercase for consistent mapping
                        std::transform(cardName.begin(), cardName.end(), cardName.begin(), ::toupper);
                        
                        ioAddress += "\"" + segment + "\".";
                    }
                    if (i == 2){ ioAddress += segment; } 
    
                    if (i == 1){ 
                        ioAddress += getLongCardType(segment); 
                        configData.type = shortCardTypeToVarConfigType(segment);
                    }
                            
                    i++;
                }
                
                configData.ioAdress = ioAddress;
                configData.processVariable = processVariable;
                configData.comment = cardName; // Use card name as comment
                
                // Add the config data to the appropriate card's node
                tempNodeMap[cardName].values.push_back(configData);
                // Set the comment for the node to the card name
                tempNodeMap[cardName].comment = cardName;
            }
        }
        
        // Convert the map to a vector
        std::vector<brVarConfigNode> configNodes;
        for (const auto& [cardName, node] : tempNodeMap) {
            configNodes.push_back(node);
        }
        
        return configNodes;
    }





}
