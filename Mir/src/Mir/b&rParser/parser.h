#pragma once

#include <string>
#include <vector>
#include <ios>

#include <map>
#include "brTypes.h"
namespace Mir {
    class brParser
    {
    public:
        brParser(/* args */);
        ~brParser();
        std::string readFile(const std::string& path);
        std::vector<std::string> readDatatypeFile(const std::string& path);
        
        brTyp readDatafile999999(const std::string& path);
        std::map<std::string, std::vector<brTyp>> readPlcDataCsv(const std::string& path);

        void writeFile(const std::string& path, const std::string& content, std::ios_base::openmode mode);
        void writeDummyData();

        std::vector<std::string> splitString(const std::string& str, const std::string& delimiter);
        std::vector<std::string> splitStringStruct(const std::string& str);
        std::stringstream dataOut();

        std::string removeSpaces(std::string& str);

        void setDataStr();
        
        brTyp& getData() { return m_testData; }
        
        void setData(const brTyp& data) { m_testData = data; }


    private:
        brTyp m_testData;
    };
}
