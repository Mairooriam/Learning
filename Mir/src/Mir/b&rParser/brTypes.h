#pragma once

#include <string>
#include <vector>
#include <ios>
#include <random>
#include <map>

namespace Mir {


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

        brStructNode& clear() {
            id = 0;
            name.clear();
            data.clear();
            return *this;
        }

        // Constructor
        brStructNode() : id(0) {} // We'll compute this based on contents

        void updateId() {
            size_t hash = 0;
            std::hash<std::string> str_hasher;
            
            for (const auto& item : data) {
                // Hash based on data content since brStructData doesn't have id
                size_t item_hash = str_hasher(item.name + item.type + item.value + item.comment);
                hash ^= item_hash + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            
            // Add name to the mix
            hash ^= str_hasher(name) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            
            id = hash;
        }

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
        size_t id;  
        std::string comment;
        std::vector<brStructNode> nodes;
    
            // Constructor that generates a unique ID
            brStructCollection() : id(0) {}

            // Assignment operator
            brStructCollection& operator=(const brStructCollection& other) {
                if (this != &other) {
                    comment = other.comment;
                    nodes = other.nodes;
                    updateId();  // Update ID after assignment
                }
                return *this;
            }

        // Update ID based on nodes
        void updateId() {
            size_t hash = 0;
            for (const auto& node : nodes) {
                hash ^= node.id + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            // Add comment to the mix
            std::hash<std::string> str_hasher;
            hash ^= str_hasher(comment) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            
            id = hash;
        }

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
        size_t id; 
        std::vector<brStructCollection> typ;
        mutable std::string m_cachedString;
        mutable bool m_isDirty = true;

        brTyp() : id(0) {}

        void updateId() {
            size_t hash = 0;
            for (const auto& collection : typ) {
                hash ^= collection.id + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            id = hash;
        }

        const brStructCollection getIndexData(size_t i){
            if(i < typ.size()){
                return typ[i];
            }
        }

        void deleteIndex(size_t i){
            if(i < typ.size()){
                typ.erase(typ.begin() + i);
                m_isDirty = true; 
                updateId();
            }
        }

        void push_back(const brStructCollection& collection) {
            typ.push_back(collection);
            m_isDirty = true;
            updateId();
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
            if (m_isDirty) {
                updateCachedString();
            }
            return getCachedString();
        }

        // Add this to brTyp
        void updateIdsRecursively() {
            // Update IDs at all levels
            for (auto& collection : typ) {
                for (auto& node : collection.nodes) {
                    node.updateId();  // Update node ID
                }
                collection.updateId();  // Update collection ID
            }
            updateId();  // Update type ID
        }
        
    };

    inline bool brIsComment(const std::string& str) {
        if (str.length() < 4) return false;  // Need at least (**)
        
        // Check if string starts with (* and ends with *)
        return (str.substr(0, 2) == "(*" && str.substr(str.length() - 2) == "*)");
    }

}
