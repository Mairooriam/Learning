
#include <string>
#include <iostream>
struct brStructData {
    std::string name;    ///< Name of the structure element
    std::string type;    ///< Data type of the structure element
    std::string value;   ///< Default value of the structure element
    std::string comment; ///< Comment associated with the structure element

    // Single constructor with default arguments
    brStructData(const std::string& name = "", const std::string& type = "", 
                 const std::string& value = "", const std::string& comment = "")
        : name(name), type(type), value(value), comment(comment) {}

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

        brStructCollection& clear() {
            comment.clear();
            structs.clear();
            return *this;
        }

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
    };
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