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
            
            // Find semicolon that separates type from value
            size_t semicolonPos = remainder.find(';');
            if (semicolonPos != std::string::npos) {
                // Extract type (token[1])
                result[1] = remainder.substr(0, semicolonPos);
                
                // Get string after semicolon
                std::string valueAndComment = remainder.substr(semicolonPos + 1);
                
                // Look for comment
                size_t commentStart = valueAndComment.find("(*");
                if (commentStart != std::string::npos) {
                    // Extract value (token[2])
                    result[2] = valueAndComment.substr(0, commentStart);
                    
                    // Extract comment (token[3])
                    size_t commentEnd = valueAndComment.find("*)");
                    if (commentEnd != std::string::npos) {
                        result[3] = valueAndComment.substr(commentStart + 2, 
                                                         commentEnd - commentStart - 2);
                    }
                } else {
                    // No comment, just value
                    result[2] = valueAndComment;
                }
            } else {
                // No value/comment, just type
                result[1] = remainder;
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
                        col.nodes.push_back(node);
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
                        node.data.push_back(data);
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
            number = cardName.substr(cardName.size()-2, 2);
            shortName = getShortName();
            datatype = getDataType();
        }
    };

    std::string sanitizeString(const std::string& input) {
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


    std::map<std::string, std::vector<brTyp>> brParser::readPlcDataCsv(const std::string &path){
        // std::ifstream file(path);
        // std::vector<std::string>   csvStr;
        // std::string                line;
        // std::string                cell;
        // if (!file.is_open()) {
        //     std::cerr << "Failed to open file: " << path << std::endl;
        //     return std::map<std::string, std::vector<brDataTypeNode>>();
        // }
    
        // // Check is header is valid
        // std::getline(file,line);
        // //if (line != "DEVICE,IOCARD,NAME,COMMENT"){ return std::map<std::string, std::vector<brDataTypeNode>>(); }
        // if (line != "Location,Type,BR Name,Card,Eplan name"){ return std::map<std::string, std::vector<brDataTypeNode>>(); }

        // while(std::getline(file,line))
        // {
        //     csvStr.push_back(line);
        // }

        // std::map<std::string, std::vector<brDataTypeNode>> hashtable;
        // for (const auto& str: csvStr){
        //     std::vector<std::string> tokens = splitString(str,",");
        //     if (!tokens[2].empty()){
        //         tokens[2] = sanitizeString(tokens[2]);
        //         cardInfo cardinfo(tokens[1]); // might be perforamnce pbrobme? creating new cardinfo each time. probably wont matter
        //         std::string comment = tokens[3] + "." + cardinfo.shortName + "." + cardinfo.number;
        //         brDataTypeNode node(tokens[2], cardinfo.datatype, comment);
        //         std::string key = tokens[0] + "_" + cardinfo.shortName + "_Typ";
        //         hashtable[key].push_back(node);
        //     }
        // }
        // return hashtable;
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
}
