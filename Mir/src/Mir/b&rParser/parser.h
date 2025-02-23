#pragma once

#include <string>
#include <vector>
#include <ios>

namespace Mir {

    enum brDatatypes {
        UNDEFINED = 0, 
        BOOL = 1, 
        SINT = 2, 
        INT = 3, 
        DINT = 4, 
        USINT = 5, 
        UINT = 6, 
        UDINT = 7, 
        REAL = 8, 
        STRING = 9, 
        ULINT = 10, 
        DATE_AND_TIME = 11, 
        TIME = 12, 
        DATE = 13, 
        LREAL = 14, 
        TOD = 15, 
        TIME_OF_DAY = 16, 
        BYTE = 17, 
        WORD = 18, 
        DWORD = 19, 
        LWORD = 20, 
        WSTRING = 21, 
        DT = 22, 
        LINT = 23, 
        STRUCT = 24, 
        IECDATATYPES_COUNT = 25
    };

    constexpr const char* brDatatypeToString(brDatatypes type) {
        switch (type) {
            case UNDEFINED: return "UNDEFINED";
            case BOOL: return "BOOL";
            case SINT: return "SINT";
            case INT: return "INT";
            case DINT: return "DINT";
            case USINT: return "USINT";
            case UINT: return "UINT";
            case UDINT: return "UDINT";
            case REAL: return "REAL";
            case STRING: return "STRING";
            case ULINT: return "ULINT";
            case DATE_AND_TIME: return "DATE_AND_TIME";
            case TIME: return "TIME";
            case DATE: return "DATE";
            case LREAL: return "LREAL";
            case TOD: return "TOD";
            case TIME_OF_DAY: return "TIME_OF_DAY";
            case BYTE: return "BYTE";
            case WORD: return "WORD";
            case DWORD: return "DWORD";
            case LWORD: return "LWORD";
            case WSTRING: return "WSTRING";
            case DT: return "DT";
            case LINT: return "LINT";
            case STRUCT: return "STRUCT";
            case IECDATATYPES_COUNT: return "IECDATATYPES_COUNT";
            default: return "UNKNOWN";
        }
    }


    struct brDataTypeNode {
        brDataTypeNode() = default;

        brDataTypeNode(const std::string& name, const std::string& typeStr, const std::string& comment)
            : name(name), type(typeStr), comment(comment) {
        }
        
        std::string name;
        std::string type;
        std::string comment;
    };


    struct brData {
        brData() = default;
        brData(const std::string& datatypeName)
            : datatypeName(datatypeName) {}

        void addNode(const brDataTypeNode& node) {
            data.push_back(node);
        }
        void setNode(const std::string& name, const std::string& type, const std::string& comment) {
            data.push_back(brDataTypeNode(name, type, comment));
        }

        void clear() {
            data.clear();
            datatypeName.clear();
        }

        std::string toString() const {
            std::string result = "TYPE\n";
            result += "\t" + datatypeName + " : STRUCT\n";
            for (const auto& node : data) {
            result += "\t\t" + node.name + " : " + node.type + "; (*" + node.comment + "*)\n";
            }
            result += "\nEND_STRUCT;\nEND_TYPE\n";
            return result;
        }

        // Begin and end functions for iteration
        auto begin() { return data.begin(); }
        auto end() { return data.end(); }
        auto begin() const { return data.begin(); }
        auto end() const { return data.end(); }

        // Size function to get the number of nodes
        size_t size() const { return data.size(); }

        std::string datatypeName;
        std::vector<brDataTypeNode> data;
        };

    class brDataCollection {
    public:
        void push_back(const brData& data) { m_Data.push_back(data); }
        
        // Delegation of vector operations
        auto begin() { return m_Data.begin(); }
        auto end() { return m_Data.end(); }
        auto begin() const { return m_Data.begin(); }
        auto end() const { return m_Data.end(); }
        size_t size() const { return m_Data.size(); }
        
        std::string toString() const {
            std::string result;
            for (const auto& data : m_Data) {
                result += data.toString() + "\n";
            }
            return result;
        }

    private:
        std::vector<brData> m_Data;
    };

    class brParser
    {
    public:
        brParser(/* args */);
        ~brParser();
        std::string readFile(const std::string& path);
        std::vector<std::string> readDatatypeFile(const std::string& path);
        void writeFile(const std::string& path, const std::string& content, std::ios_base::openmode mode);
        void writeDummyData();

        std::vector<std::string> splitString(const std::string& str, const std::string& delimiter);

        const brDataCollection& getData() const { return m_Data; }
        brDataCollection& getMutable() { return m_Data; }

        size_t size() const { return m_Data.size(); }
        
        void initDummyData();
    private:
        brDataCollection m_Data;
    };;

}
