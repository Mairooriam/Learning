#include <fstream>
#include <iostream>
#include <string>
#include <map>

#include "parser.h"

namespace Mir {
    

    std::vector<std::string> brParser::splitString(const std::string& str, const std::string& delimiter) {
        std::vector<std::string> tokens;
        size_t start = 0;
        size_t end = str.find(delimiter);
        
        while (end != std::string::npos) {
            std::string token = str.substr(start, end - start);
            tokens.push_back(token);
            start = end + delimiter.length();
            end = str.find(delimiter, start);
        }
        
        // Add the last token
        tokens.push_back(str.substr(start));
        return tokens;
    }

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
    //"TYPE\n\tAF101 : STRUCT\n\t\tSami1 : BOOL; (*No comment*)\n\t\tSami2 : BOOL; (*No comment*)\n\t\tSami3 : BOOL; (*No comment*)\n\t\tSami4 : BOOL; (*No comment*)\n\t\tSami5 : BOOL; (*No comment*)\n\t\tSami6 : BOOL
   
    std::vector<std::string> brParser::readDatatypeFile(const std::string &path){
          
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << path << std::endl;
            return std::vector<std::string>{"ERROR"};
        }
        std::string ingoredStrings = "TYPE";
        std::vector<std::string>   result;
        std::string                line;


        std::stringstream          lineStream(line);
        std::string                cell;
    
        while(std::getline(file,line))
        {
            result.push_back(line);
        }
        bool typeFound = false;
        bool structFound = false;
        std::vector<std::string> tokens;
        std::vector<std::string> subTokens;
        size_t iAfterTypeFound = 0;

        std::string currentName = "";
        for (const auto string : result){
            std::stringstream ss(string);
            if (structFound)
            {
                tokens = splitString(string,":");
                size_t tabCount = std::count(tokens[0].begin(), tokens[0].end(), '\t');
                
                brDataTypeNode node;
                switch (tabCount)
                {
                case 1:
                    tokens = splitString(string,":");
                    if (tokens.size() == 2) {
                        if (tokens[1].find("STRUCT"))
                        {
                            std::string varName = tokens[0];
                            std::string varType = tokens[1];
                            
                            varName.erase(0, varName.find_first_not_of(" \t"));
                            varName.erase(varName.find_last_not_of(" \t") + 1);
                            varType.erase(0, varType.find_first_not_of(" \t"));
                            varType.erase(varType.find_last_not_of(" \t;") + 1);
    
                            currentName = varName;
                        }
                        

                    } else if (tokens.size() == 1)
                    {
                        if (tokens[0].find("END_STRUCT"))
                        {
                            structFound = false;


                        }
                        
                    }
                    
                    break;
                case 2:
                    
                    tokens = splitString(string,":");
                    subTokens = splitString(tokens[1], ";");

                    tokens[0].erase(0,tokens[0].find_first_not_of(" \t"));
                    tokens[0].erase(tokens[0].find_last_not_of(" \t") + 1);
                    node.name = tokens[0];

                    subTokens[0].erase(0,subTokens[0].find_first_not_of(" \t"));
                    subTokens[0].erase(subTokens[0].find_last_not_of(" \t") + 1);
                    node.type = subTokens[0];

                    if (subTokens[1].size() > 0)
                    {
                        subTokens[1].erase(0,subTokens[1].find_first_not_of(" \t"));
                        subTokens[1].erase(subTokens[1].find_last_not_of(" \t") + 1);
                        subTokens[1] = subTokens[1].substr(subTokens[1].find("(*") + 2);
                        subTokens[1] = subTokens[1].substr(0, subTokens[1].find("*)"));
                        node.comment = subTokens[1];
                    } else{
                        node.comment = "";
                    };
                    
                    m_Data[currentName].push_back(node);
                    
                    break;
                default:
                    break;
                }
            }

            
            
            if (string == "TYPE")
            {
                typeFound = true;
            } else if (string.find("STRUCT"))
            {
                structFound = true;
            }
            
            
        }
        
        // This checks for a trailing comma with no data after it.
        if (!lineStream && cell.empty())
        {
            // If there was a trailing comma then add an empty element.
            result.push_back("");
        }

        file.close();
        return result;

    }

    std::map<std::string, std::vector<brDataTypeNode>> brParser::readDataTypeFileOneLineAtaTime(const std::string &path)
    {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << path << std::endl;
            return std::map<std::string, std::vector<brDataTypeNode>>();
        }

        std::string                 line;
        size_t                      LineIndex = 1;
        std::string                 lastLine;
        std::vector<std::string>    tokens;
        bool                        typeFound = 0;

        std::stringstream          lineStream(line);
        std::string                cell;
        std::map<std::string, std::vector<brDataTypeNode>> outputMap;
        while(std::getline(file,line))
        {
            LineIndex++;

            if (line == ""){
                lastLine = "";
                continue;
            }
             

            if (line == "TYPE"){
                typeFound = true;
            } 

             
                
            if (line.length() >= 2 && line[0] == '(' && line[1] == '*')
            {
                // FOR NEW IMPLEMENTATION
                std::map<std::string, std::vector<brDataTypeNode>> temp_data;



                std::string comment = line.substr(2); // Remove (*
                if (comment.find("*)") != std::string::npos) {
                    comment = comment.substr(0, comment.find("*)"));
                }
                brDataTypeNode node("Comment", "", "", std::to_string(LineIndex));
                std::string key = "comment" + std::to_string(LineIndex);
                outputMap[key].push_back(node);

                // FOR new implementation
                temp_data[key].push_back(node);
                m_test_dataVector.push_back(temp_data);
            }

            
            if (typeFound)
            {
                tokens = splitString(removeSpaces(line),":");

                if(tokens.size() == 1) continue;
                
                if (tokens.size() > 2) {
                    MIR_CORE_ASSERT(tokens.size() > 2, "There shouldnt be more than one : separator ( 2 tokens) update code! Maybe not valid input format");

                    continue;
                }

                // HANDLE STRUCT
                if (tokens[1] == "STRUCT")
                {
                    // FOR NEW IMPLEMENTATION
                    std::map<std::string, std::vector<brDataTypeNode>> temp_data;


                    std::string structName = tokens[0];
                    brDataTypeNode node("Line", std::to_string(LineIndex), "");
                    outputMap[structName].push_back(node);

                    // FOR NEW
                    temp_data[structName].push_back(node);

                    bool structFound = true;
                    MIR_CORE_INFO("Hit struct at {0}", LineIndex);
                    while (std::getline(file, line))
                    {
                        LineIndex++;

                        if (line == "") continue;

                        if (line.find("END_STRUCT") != std::string::npos)
                        {
                            structFound = false;
                            m_test_dataVector.push_back(temp_data); // FOR NEW IMPL
                            MIR_CORE_INFO("Hit end struct at {0}", LineIndex);
                            break;
                        }

                    
                        if (structFound)
                        {
                            tokens = splitStringStruct(removeSpaces(line));
                            brDataTypeNode node(tokens[0], tokens[1], tokens[2]);
                            outputMap[structName].push_back(node);


                            // FOR NEW IMPL
                            temp_data[structName].push_back(node);
                        } else{
                            MIR_CORE_ASSERT(true,"Shouldnt happen. fix before using");
                        }
                    }
                    
                } 
                
                // HANDLE ENUM
                if (tokens[1] == "")
                {
                    MIR_CORE_ASSERT(true, "Add implementation for enums!")
                }
                
            }
            lastLine = line;            
        }
        return outputMap;
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
                
               


                
                MIR_INFO("TYPE FOUND");
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


    std::map<std::string, std::vector<brDataTypeNode>> brParser::readPlcDataCsv(const std::string &path){
        std::ifstream file(path);
        std::vector<std::string>   csvStr;
        std::string                line;
        std::string                cell;
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << path << std::endl;
            return std::map<std::string, std::vector<brDataTypeNode>>();
        }
    
        // Check is header is valid
        std::getline(file,line);
        //if (line != "DEVICE,IOCARD,NAME,COMMENT"){ return std::map<std::string, std::vector<brDataTypeNode>>(); }
        if (line != "Location,Type,BR Name,Card,Eplan name"){ return std::map<std::string, std::vector<brDataTypeNode>>(); }

        while(std::getline(file,line))
        {
            csvStr.push_back(line);
        }

        std::map<std::string, std::vector<brDataTypeNode>> hashtable;
        for (const auto& str: csvStr){
            std::vector<std::string> tokens = splitString(str,",");
            if (!tokens[2].empty()){
                tokens[2] = sanitizeString(tokens[2]);
                cardInfo cardinfo(tokens[1]); // might be perforamnce pbrobme? creating new cardinfo each time. probably wont matter
                std::string comment = tokens[3] + "." + cardinfo.shortName + "." + cardinfo.number;
                brDataTypeNode node(tokens[2], cardinfo.datatype, comment);
                std::string key = tokens[0] + "_" + cardinfo.shortName + "_Typ";
                hashtable[key].push_back(node);
            }
        }
        return hashtable;
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

    void brParser::writeDummyData(){
        std::string datatest = this->toString();
        writeFile("C:\\projects\\OpcUa_Sample\\Logical\\Types.typ",datatest,std::ios_base::app);
    }


    void brParser::initDummyData()
    {
        for (int i = 1; i <= 10; ++i) {
            std::string nodeName = "Sami" + std::to_string(i);
            Mir::brDataTypeNode node(nodeName, "BOOL", "No comment");
            m_Data["DUMMY_DATA"].push_back(node);
        }
    }
}
