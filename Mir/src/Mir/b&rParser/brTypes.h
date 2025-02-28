#pragma once

#include <string>
#include <vector>
#include <ios>

#include <map>

namespace Mir {
        // Data type suggestions
    const std::vector<std::string_view> brDataTypesSuggestions = {
            "BOOL", "SINT", "INT", "DINT", "USINT", "UINT", "UDINT", "REAL",
            "STRING", "ULINT", "DATE_AND_TIME", "TIME", "DATE", "LREAL",
            "TOD", "TIME_OF_DAY", "BYTE", "WORD", "DWORD", "LWORD",
            "WSTRING", "DT", "LINT", "STRUCT"
        };
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

    struct brStructData {
        std::string name;
        std::string type;
        std::string value;
        std::string comment;

        std::string toString() const {
            std::stringstream ss;
            ss << name;
            if (!type.empty()) ss << " : " << type;
            if (!value.empty()) ss << " := " << value;
            if (!comment.empty()) ss << "; (*" << comment << "*)";
            return ss.str();
        }
    };

    struct brStructNode {
        size_t id;
        std::string name;
        std::vector<brStructData> data;

        std::string toString() const {
            std::stringstream ss;
            ss << name << " :\t" << "STRUCT";
            for (const auto& d : data) {
                ss << "\n\t\t " << d.toString();
            }
            ss << "\nEND_STRUCT;";
            return ss.str();
        }
    };

    struct brStructCollection {
        std::string comment;
        std::vector<brStructNode> nodes;
    
        std::string toString() const {
            std::stringstream ss;
            if (!comment.empty()) {
                ss << "(*" << comment << "*)";
            }
    
            ss << "\nTYPE";
            for (const auto& node : nodes) {
                ss << "\n" << node.toString();
            }
            ss << "\nEND_TYPE\n";
            
            return ss.str();
        }
    };

    struct brTyp {
        std::vector<brStructCollection> typ;
        mutable std::string m_cachedString;
        mutable bool m_isDirty = true;
        
        void push_back(const brStructCollection& collection) {
            typ.push_back(collection);
            m_isDirty = true;
        }
        // Get the number of collections
        size_t size() const { return typ.size(); }

        // Get cached string without updating
        const std::string& getCachedString() const {
            return m_cachedString;
        }

        // Explicit update method
        void updateCachedString() const {
            std::stringstream ss;
            for (const auto& collection : typ) {
                ss << collection.toString();
            }
            m_cachedString = ss.str();
            m_isDirty = false;
        }

            // Check if cache needs update
        bool isDirty() const {
            return m_isDirty;
        }

        // Mark cache as needing update
        void markDirty() {
            m_isDirty = true;
        }

        std::string toString() const {
            return getCachedString();
        }
    };

    inline bool brIsComment(const std::string& str) {
        if (str.length() < 4) return false;  // Need at least (**)
        
        // Check if string starts with (* and ends with *)
        return (str.substr(0, 2) == "(*" && str.substr(str.length() - 2) == "*)");
    }

}
