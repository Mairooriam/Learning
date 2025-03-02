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
        
        setData(result);
        m_testData.updateCachedString();
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
            row[Location];
            row[Type];
            row[Name];
            row[IoCardName];

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


    void brParser::readAndupdateFromCSV(std::string path, std::string header)
    { 
        auto csvData = readPlcDataCsv(path, header);
        brStructCollection test1 = parseCsvIntoBrCollection(csvData);
        addCollection(test1);
    }
}
