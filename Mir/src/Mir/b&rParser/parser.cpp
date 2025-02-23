#include <fstream>
#include <iostream>
#include <string>

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


    void writeDummyData()
    {

    }

    brParser::brParser()
    {
        //initDummyData();
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
    
                            m_Data.datatypeName = varName;
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

                    if (subTokens[1].size() < 0)
                    {
                        subTokens[1].erase(0,subTokens[1].find_first_not_of(" \t"));
                        subTokens[1].erase(subTokens[1].find_last_not_of(" \t") + 1);
                        subTokens[1] = subTokens[1].substr(subTokens[1].find("(*") + 2);
                        subTokens[1] = subTokens[1].substr(0, subTokens[1].find("*)"));
                        node.comment = subTokens[1];
                    } else{
                        node.comment = "";
                    };
                    

                    m_Data.addNode(node);
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
        std::string datatest = m_Data.toString();
        writeFile("C:\\projects\\OpcUa_Sample\\Logical\\Types.typ",datatest,std::ios_base::app);
    }

    void brParser::initDummyData(){
        m_Data.datatypeName = "AF101";
        for (int i = 1; i <= 10; ++i) {
            std::string nodeName = "Sami" + std::to_string(i);
            Mir::brDataTypeNode node(nodeName, "BOOL", "No comment");
            m_Data.addNode(node);
        }
    }
}
