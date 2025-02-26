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
        std::map<std::string, std::vector<brDataTypeNode>> readDataTypeFileOneLineAtaTime(const std::string& path);
        std::map<std::string, std::vector<brDataTypeNode>> readPlcDataCsv(const std::string& path);

        void writeFile(const std::string& path, const std::string& content, std::ios_base::openmode mode);
        void writeDummyData();

        std::vector<std::string> splitString(const std::string& str, const std::string& delimiter);
        std::vector<std::string> splitStringStruct(const std::string& str);
        std::string removeSpaces(std::string& str);

        const std::map<std::string, std::vector<brDataTypeNode>>& getData() const { return m_Data; }
        std::map<std::string, std::vector<brDataTypeNode>>& getMutable() { return m_Data; }

        size_t size() const { return m_Data.size(); }
        

        std::string toString() const {
            std::string result;
            for (const auto& [name, nodes] : m_Data) {
                result += "TYPE\n";
                result += "\t" + name + " : STRUCT\n";
                for (const auto& node : nodes) {
                    result += "\t\t" + node.name + " : " + node.type + "; (*" + node.comment + "*)\n";
                }
                result += "\nEND_STRUCT;\nEND_TYPE\n";
            }
            return result;
        }

        void mergeMaps(const std::map<std::string, std::vector<brDataTypeNode>>& other) {
            for (const auto& [key, values] : other) {
                // Insert or append to existing vector
                m_Data[key].insert(m_Data[key].end(), values.begin(), values.end());
            }
        }


        void initDummyData();
    private:
        std::map<std::string, std::vector<brDataTypeNode>> m_Data;
    };

}
