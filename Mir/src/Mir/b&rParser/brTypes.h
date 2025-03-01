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
        std::vector<brStructData> values;

        brStructNode& clear() {
            name.clear();
            values.clear();
            return *this;
        }

        std::string toString() const {
            std::stringstream ss;
            ss << name << " :\t" << "STRUCT";
            for (const auto& value : values) {
                ss << "\n\t\t " << value.toString();
            }
            ss << "\nEND_STRUCT;";
            return ss.str();
        }
    };

    struct brStructCollection {
    std::string comment;
    std::vector<brStructNode> structs;

        std::string toString() const {
            std::stringstream ss;
            if (!comment.empty()) {
                ss << "(*" << comment << "*)";
            }
    
            ss << "\nTYPE";
            for (const auto& child : structs) {
                ss << "\n" << child.toString();
            }
            ss << "\nEND_TYPE\n";
            
            return ss.str();
        }
    };

    struct brTyp {
        std::vector<brStructCollection> collections;
        mutable std::string m_cachedString;
        mutable bool m_isDirty = true;
        std::unordered_map<size_t, int> copyCounters;

        brStructData& getValueAt(size_t i, size_t j, size_t k) {
            return collections[i].structs[j].values[k];
        }

        const brStructCollection getIndexData(size_t i) const {
            if(i < collections.size()){
                return collections[i];
            }
            return brStructCollection{};
        }

        void duplicateCollectionAt(size_t i) {
            if(i >= collections.size()) return;
            
            brStructCollection copy = collections[i];
            
            // Increment copy counter for this index
            copyCounters[i]++;
            int copyNum = copyCounters[i];
            
            // Add "_copyN" to collection comment
            copy.comment = copy.comment + "_copy" + std::to_string(copyNum);
            
            // Add "_copyN" to each struct name and values
            for (auto& str : copy.structs) {
            str.name = str.name + "_copy" + std::to_string(copyNum);
            for (auto& val : str.values) {
                val.name = val.name + "_copy" + std::to_string(copyNum);
            }
            }
            
            collections.push_back(copy);
            m_isDirty = true;
        }

        void duplicateStructAt(size_t collectionIndex, size_t structIndex) {
            if(collectionIndex >= collections.size() || 
               structIndex >= collections[collectionIndex].structs.size()) return;
            
            brStructNode copy = collections[collectionIndex].structs[structIndex];
            
            // Increment copy counter for this struct
            size_t id = copy.id;
            copyCounters[id]++;
            int copyNum = copyCounters[id];
            
            // Add "_copyN" to struct name and values
            copy.name = copy.name + "_copy" + std::to_string(copyNum);
            for (auto& val : copy.values) {
            val.name = val.name + "_copy" + std::to_string(copyNum);
            }
            
            collections[collectionIndex].structs.push_back(copy);
            m_isDirty = true;
        }

        void deleteCollectionAt(size_t i){
            if(i < collections.size()){
            collections.erase(collections.begin() + i);
            m_isDirty = true; 
            }
        }

        void deleteStructAt(size_t collectionIndex, size_t structIndex) {
            if(collectionIndex < collections.size() && 
               structIndex < collections[collectionIndex].structs.size()) {
            collections[collectionIndex].structs.erase(
                collections[collectionIndex].structs.begin() + structIndex);
            m_isDirty = true;
            }
        }


        void push_back(const brStructCollection& collection) {
            collections.push_back(collection);
            m_isDirty = true;
        }
        // Get the number of collections
        size_t size() const { return collections.size(); }

        // Get cached string without updating
        const std::string& getCachedString() const {
            return m_cachedString;
        }

        // Explicit update method
        void updateCachedString() const {
            std::stringstream ss;
            for (const auto& collection : collections) {
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
        
    };

    inline bool brIsComment(const std::string& str) {
        if (str.length() < 4) return false;  // Need at least (**)
        
        // Check if string starts with (* and ends with *)
        return (str.substr(0, 2) == "(*" && str.substr(str.length() - 2) == "*)");
    }

}
