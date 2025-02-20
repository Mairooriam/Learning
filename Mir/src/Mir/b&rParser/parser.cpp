#include <fstream>
#include <iostream>
#include <string>

#include "parser.h"

namespace Mir {
    




    void writeDummyData()
    {

    }

    brParser::brParser()
    {
        initDummyData();
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
            Mir::brDataTypeNode node(nodeName, Mir::brDatatypes::BOOL, "No comment");
            m_Data.addNode(node);
        }
    }
}
