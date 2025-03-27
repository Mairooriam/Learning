#pragma once

#include <string>
#include <vector>
#include <ios>
#include <random>
#include <map>


#include <iostream>
#include <variant>
namespace Mir {
   
        ////////////////////////////////////////
    //// SELECTABLE ELEMENT ID MAP /////////
    ////////////////////////////////////////
    enum class typTypes {
        None = 0,
        Collection,
        Struct,
        Value
    };
    
    struct CollectionInfo{
        size_t i;
        std::string name;
    };
    
    struct StructInfo{
        size_t i;
        size_t j;
        std::string name;
    };
    
    struct ValueInfo{
        size_t i;
        size_t j;
        size_t k;
        std::string name;
    };
    
    inline std::string typTypeToString(typTypes type) {
        switch (type) {
            case typTypes::None: return "None";
            case typTypes::Collection: return "Collection";
            case typTypes::Struct: return "Struct";
            case typTypes::Value: return "Value";
            default: return "Unknown";
        }
    }

    // Move the class definition outside of main
    struct selectableElement {
        typTypes type = typTypes::None;
        std::variant<std::monostate, CollectionInfo, StructInfo, ValueInfo> data;
        
        selectableElement() = default;
    

        template<typename T>
        const T* GetData() const {
            if (std::holds_alternative<T>(data)) {
                return &std::get<T>(data);
            }
            return nullptr;
        }
        template<typename T>
        selectableElement(typTypes type_, T&& data_)
            : type(type_), data(std::forward<T>(data_)) {}

        bool isType(typTypes _type) { return _type == type; }

        void toString() {
            std::cout << "Type: " << typTypeToString(type) << " - ";
            std::visit([](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, std::monostate>)
                    std::cout << "No data";
                else if constexpr (std::is_same_v<T, CollectionInfo>)
                    std::cout << "Collection: " << arg.name << "i:" << arg.i;
                else if constexpr (std::is_same_v<T, StructInfo>)
                    std::cout << "Struct: " << arg.name << ", i: " << arg.i << ", j: " << arg.j;
                else if constexpr (std::is_same_v<T, ValueInfo>)
                    std::cout << "Value at " << arg.name << ", i: " << arg.i << ", j: " << arg.j << ", k: "<< arg.k;
            }, data);
            std::cout << std::endl;
        }
    };
    ////////////////////////////////////////
    //// SELECTABLE ELEMENT ID MAP /////////
    ////////////////////////////////////////





    inline std::string getShortCardType(std::string cardType) {
        if (cardType.find("DigitalInput") != std::string::npos) return "Di";
        if (cardType.find("DigitalOutput") != std::string::npos) return "Do";
        if (cardType.find("AnalogInput") != std::string::npos) return "Ai";
        if (cardType.find("AnalogOutput") != std::string::npos) return "Ao";
        return "Unknown";
    }
    
    inline std::string getLongCardType(std::string shortCardType) {
        if (shortCardType == "Di") return "DigitalInput";
        if (shortCardType == "Do") return "DigitalOutput";
        if (shortCardType == "Ai") return "AnalogInput";
        if (shortCardType == "Ao") return "AnalogOutput";
        return "Unknown";
    }

    inline std::string shortCardTypeToType(std::string shortCardType) {
        if (shortCardType == "Di") return "BOOL";
        if (shortCardType == "Do") return "BOOL";
        if (shortCardType == "Ai") return "REAL";
        if (shortCardType == "Ao") return "REAL";
        return "UNKNOWN";
    }
    inline std::string longCardTypeToVarConfigType(std::string longCardType){
        if (longCardType == "DigitalInput") return "%IX";
        if (longCardType == "DigitalOutput") return "%QX";
        if (longCardType == "AnalogInput") return "%IW";
        if (longCardType == "AnalogOutput") return "%QW";
        return "UNKNOWN";
    }

    inline std::string shortCardTypeToVarConfigType(std::string shortCardType){
        if (shortCardType == "Di") return "%IX";
        if (shortCardType == "Do") return "%QX";
        if (shortCardType == "Ai") return "%IW";
        if (shortCardType == "Ao") return "%QW";
        return "UNKNOWN";
    }


    inline std::string varConfigTypeToLongCardType(std::string varConfigType){
        if (varConfigType == "%IX") return "DigitalInput";
        if (varConfigType == "%QX") return "DigitalOutput";
        if (varConfigType == "%IW") return "AnalogInput";
        if (varConfigType == "%QW") return "AnalogOutput";
        return "Unknown";
    }

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

    /**
     * @brief Represents a structure data element with name, type, value and comment
     */
    struct brStructData {
        
        std::string name;    ///< Name of the structure element
        std::string type;    ///< Data type of the structure element
        std::string value;   ///< Default value of the structure element
        std::string comment; ///< Comment associated with the structure element

        /**
         * @brief Default constructor
         */
        brStructData() = default;

        /**
         * @brief Constructor to initialize all fields
         * @param name The name of the structure element
         * @param type The data type of the structure element (defaults to empty string)
         * @param value The default value of the element (defaults to empty string)
         * @param comment A comment for the structure element (defaults to empty string)
         */
        brStructData(const std::string& name, const std::string& type = "", 
                     const std::string& value = "", const std::string& comment = "")
            : name(name), type(type), value(value), comment(comment) {}

        /**
         * @brief Converts the structure data to a string representation
         * @return A string representation of the structure data
         */
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

        // Default constructor
        brStructNode() = default;

        brStructNode(const std::string& name, const std::vector<brStructData>& values = {})
        : name(name), values(values) {}

        // Constructor with parameters
        brStructNode(size_t id, const std::string& name, const std::vector<brStructData>& values = {})
            : id(id), name(name), values(values) {}

        void push_back(const brStructData& data) {
            values.push_back(data);
        }

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

        brStructCollection& clear() {
            comment.clear();
            structs.clear();
            return *this;
        }

        // Default constructor
        brStructCollection() = default;

        // Constructor with parameters to specify contents
        brStructCollection(const std::string& comment, const std::vector<brStructNode>& structs = {})
            : comment(comment), structs(structs) {}

        // Iterator methods
        auto begin() { return structs.begin(); }
        auto end() { return structs.end(); }
        auto begin() const { return structs.begin(); }
        auto end() const { return structs.end(); }
        
        // if you want to brSturctCollection[i] instead of brsturctollection.structs[i]
        //brStructNode& operator[](size_t index) { return structs[index]; }
        //const brStructNode& operator[](size_t index) const { return structs[index]; }

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
    };;
    struct brTyp {
        std::vector<brStructCollection> collections;
        std::string m_cachedString;
        bool m_isDirty = true;
        std::unordered_map<size_t, int> copyCounters;

        bool empty() { 
            if (collections.empty()) { return true; }
            else { return false; }
        }    

        // Clear all data and reset state
        brTyp& clear() {
            std::vector<brStructCollection>().swap(collections);
            std::string().swap(m_cachedString);
            std::unordered_map<size_t, int>().swap(copyCounters);
            m_isDirty = true;
            return *this;
        }

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

        brStructNode duplicateStructAt(size_t collectionIndex, size_t structIndex) {
            if(collectionIndex >= collections.size() || 
               structIndex >= collections[collectionIndex].structs.size()) 
            return brStructNode{};
            
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
            
            return copy;
        }

        void insertStructAt(size_t collectionIndex, size_t structIndex, brStructNode structToInsert) {
            if(collectionIndex >= collections.size()) 
            return;
            
            // If structIndex is out of range, append at the end
            if(structIndex > collections[collectionIndex].structs.size()) {
            collections[collectionIndex].structs.push_back(structToInsert);
            } else {
            collections[collectionIndex].structs.insert(
                collections[collectionIndex].structs.begin() + structIndex, 
                structToInsert
            );
            }
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

        void deleteValueAt(size_t collectionIndex, size_t structIndex, size_t valueIndex) {
            if(collectionIndex < collections.size() && 
               structIndex < collections[collectionIndex].structs.size() &&
               valueIndex < collections[collectionIndex].structs[structIndex].values.size()) {
            collections[collectionIndex].structs[structIndex].values.erase(
                collections[collectionIndex].structs[structIndex].values.begin() + valueIndex);
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
        void updateCachedString()  {
            if (m_isDirty == true)
            {
                std::stringstream ss;
                for (const auto& collection : collections) {
                    ss << collection.toString();
                }
                m_cachedString = ss.str();
                m_isDirty = false;
            }
        }

            // Check if cache needs update
        bool isDirty() const {
            return m_isDirty;
        }

        // Mark cache as needing update
        void markDirty() {
            m_isDirty = true;
        }

        std::string toString() {
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




    struct brVarConfigData {

        std::string ioAdress{};
        std::string processVariable{};    
        std::string type{}; 
        std::string comment{}; 
        brVarConfigData() = default;

        std::string toString() const {
            std::stringstream ss;
            ss << processVariable + " AT "; 
            ss << type + ".";
            ss << ioAdress + "; ";
            ss << "(*" << comment << "*)";
            return ss.str();
        }
    };
    struct brVarConfigNode {
        std::string comment;
        std::vector<brVarConfigData> values;

        brVarConfigNode& clear() {
            comment.clear();
            values.clear();
            return *this;
        }
        
        // Iterator methods for range-based for loop
        auto begin() { return values.begin(); }
        auto end() { return values.end(); }
        auto begin() const { return values.begin(); }
        auto end() const { return values.end(); }

        std::string toString() const {
            std::stringstream ss;
            ss << "(*" << comment << "*)";
            ss << "\nVAR_CONFIG" ;
            for (const auto& value : values) {
                ss << "\n\t" << value.toString();
            }
            ss << "\nEND_VAR\n";
            return ss.str();
        }

        std::string toStringPlain() const {
            std::stringstream ss;
            for (const auto& value : values) {
                ss << "\t" << value.toString() << "\n";
            }
            return ss.str();
        }
    };

    struct brVarConfigCollection {
        std::vector<brVarConfigNode> config;
        std::string m_cachedString;
        bool m_isDirty = true;
        //std::unordered_map<size_t, int> copyCounters;
        bool empty(){ return config.empty(); }
        brVarConfigCollection& clear() {
            config.clear();
            m_cachedString.clear();
            return *this;
        }
        
        bool isDirty() const {
            return m_isDirty;
        }

        // Mark cache as needing update
        void markDirty() {
            m_isDirty = true;
        }
        
        void insertNodeAt(size_t nodeIndex, brVarConfigNode nodeToInsert) {
            // If nodeIndex is out of range, append at the end
            if(nodeIndex > config.size()) {
                config.push_back(nodeToInsert);
            } else {
                config.insert(
                    config.begin() + nodeIndex,
                    nodeToInsert
                );
            }
            m_isDirty = true;
        }
        
        std::string toString() const {
            std::stringstream ss;
            for (const auto& value : config) {
                ss << value.toString();
            }
            return ss.str();
        }

        std::string toStringPlain() const {
            std::stringstream ss;
            for (const auto& value : config) {
                ss << value.toStringPlain();
            }
            return ss.str();
        }

        size_t size() const { return config.size(); }

        void updateCachedString() {
            if (m_isDirty == true) {
                m_cachedString = toString();
                m_isDirty = false;
            }
        }

        void push_back(brVarConfigNode varConfigNode) {
            config.push_back(varConfigNode);
        }

        // Iterator methods for range-based for loop
        auto begin() { return config.begin(); }
        auto end() { return config.end(); }
        auto begin() const { return config.begin(); }
        auto end() const { return config.end(); }
    };;



}
