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

        void clear(){ m_testData.clear(); }
        
        brTyp readDatafile999999(const std::string& path);
        std::vector<std::vector<std::string>> readPlcDataCsv(const std::string &path, const std::string& header);

        void writeFile(const std::string& path, const std::string& content, std::ios_base::openmode mode);

        brStructCollection parseCsvIntoBrCollection(std::vector<std::vector<std::string>>& csvStr);
        void readAndupdateFromCSV(std::string path, std::string header);

        std::vector<std::string> splitString(const std::string& str, const std::string& delimiter);
        std::vector<std::string> splitStringStruct(const std::string& str);


        std::string removeSpaces(std::string& str);
        std::string sanitizeString(const std::string& input);
        brTyp& getData() { return m_testData; }
        
        void setData(const brTyp& data) { m_testData = data; }
        void addCollection(brStructCollection& col) { m_testData.push_back(col); }

    private:
        brTyp m_testData;
    };
}
