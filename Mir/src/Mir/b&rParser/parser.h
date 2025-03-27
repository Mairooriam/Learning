#pragma once

#include <string>
#include <vector>
#include <ios>

#include <map>
#include <unordered_map>
#include "brTypes.h"



namespace Mir {




    struct MirClipboard {
        enum class ContentType {
            None,
            String,
            BrCollection,
            BrStruct,
            VarConfigCollection,
            VarConfigNode
        };
        
        std::string stringContent;
        
        // Add these fields to store the actual copied data
        brStructCollection collectionData;
        brStructNode structData;
        brVarConfigCollection varConfigCollectionData;
        brVarConfigNode varConfigNodeData;
        
        // Indices for the copied item (if needed for reference)
        size_t collectionIndex = 0;
        size_t structIndex = 0;
        size_t varConfigIndex = 0;
        
        ContentType contentType = ContentType::None;
        
        // Check content type methods
        bool isCollection() const {
            return contentType == ContentType::BrCollection;
        }
        
        bool isStruct() const {
            return contentType == ContentType::BrStruct;
        }
        
        bool isString() const {
            return contentType == ContentType::String;
        }
        
        bool isVarConfigCollection() const {
            return contentType == ContentType::VarConfigCollection;
        }
        
        bool isVarConfigNode() const {
            return contentType == ContentType::VarConfigNode;
        }
        
        bool isEmpty() const {
            return contentType == ContentType::None;
        }
        
        // Copying methods
        void copyCollection(const brStructCollection& collection, size_t index) {
            collectionData = collection;
            collectionIndex = index;
            contentType = ContentType::BrCollection;
        }
        
        void copyStruct(const brStructNode& structure, size_t colIdx, size_t strIdx) {
            structData = structure;
            collectionIndex = colIdx;
            structIndex = strIdx;
            contentType = ContentType::BrStruct;
        }
        
        void copyVarConfigCollection(const brVarConfigCollection& collection) {
            varConfigCollectionData = collection;
            contentType = ContentType::VarConfigCollection;
        }
        
        void copyVarConfigNode(const brVarConfigNode& node, size_t idx) {
            varConfigNodeData = node;
            varConfigIndex = idx;
            contentType = ContentType::VarConfigNode;
        }
        
        // ToString implementation
        std::string toString() const {
            switch (contentType) {
                case ContentType::String:
                    return stringContent;
                case ContentType::BrCollection:
                    return collectionData.toString();
                case ContentType::BrStruct:
                    return structData.toString();
                case ContentType::VarConfigCollection:
                    return varConfigCollectionData.toString();
                case ContentType::VarConfigNode:
                    return varConfigNodeData.toString();
                default:
                    return "";
            }
        }
        
        // Methods for pasting
        void pasteToCollection(brTyp& data) {
            if (contentType == ContentType::BrCollection) {
                data.push_back(collectionData);
            }
        }
    
        void pasteStructToCollection(brTyp& data, size_t targetCollectionIndex) {
            if (contentType == ContentType::BrStruct && 
                targetCollectionIndex < data.size()) {
                data.insertStructAt(targetCollectionIndex, 
                                    data.collections[targetCollectionIndex].structs.size(),
                                    structData);
            }
        }
        
        void pasteToVarConfigCollection(brVarConfigCollection& data) {
            if (contentType == ContentType::VarConfigCollection) {
                // Append all nodes from the copied collection
                for (const auto& node : varConfigCollectionData) {
                    data.push_back(node);
                }
            } else if (contentType == ContentType::VarConfigNode) {
                data.push_back(varConfigNodeData);
            }
        }
        
        // void pasteVarConfigNodeAt(brVarConfigCollection& data, size_t targetCollectionIndex) {
        //     if (contentType == ContentType::VarConfigNode && targetCollectionIndex <= data.size()) {
        //         data.insertNodeAt(data.begin() + targetCollectionIndex, varConfigNodeData);
        //     }
        // }
    };

    class brParser
    {
    public:
        brParser(/* args */);
        ~brParser();
        std::string readFile(const std::string& path);

        void clear() { clearTyp(); clearVarConfig();}
        void clearTyp(){ m_testData.clear(); }
        void clearVarConfig() { m_varConfig.clear(); }

        brTyp readDatafile999999(const std::string& path);
        std::vector<std::vector<std::string>> readPlcDataCsv(const std::string &path, const std::string& header);

        void writeFile(const std::string& path, const std::string& content, std::ios_base::openmode mode);

        brStructCollection parseCsvIntoBrCollection(std::vector<std::vector<std::string>>& csvStr);
        brVarConfigNode parseSturctToConfig(const brStructNode& col);
        brVarConfigNode parseCollectionToConfig(const brStructCollection& col);
        std::vector<brVarConfigNode> parseCollectionToConfigMap(const brStructCollection& col);
        void readAndupdateFromCSV(std::string path, std::string header);

        std::vector<std::string> splitString(const std::string& str, const std::string& delimiter);
        std::vector<std::string> splitStringStruct(const std::string& str);
        brVarConfigCollection readBrVarConfig(const std::string& path);

        std::string removeSpaces(std::string& str);
        std::string sanitizeString(const std::string& input);
        
        brTyp& getData() { return m_testData; }
        brVarConfigCollection& getVarConfig() { return m_varConfig; }
    
        void setData(const brTyp& data) { m_testData = data; }
        void setVarConfig(const brVarConfigCollection& varConfig) { m_varConfig = varConfig; }
            
        void addVarConfigNode(brVarConfigNode varConfigNode) { m_varConfig.push_back(varConfigNode); }
        inline void addStructToCol(size_t i, brStructNode structNode) { m_testData.collections[i].structs.push_back(structNode); }
        inline void addDataToStruct(brStructData sturctData, size_t i, size_t j) { m_testData.collections[i].structs[j].values.push_back(sturctData); }
        void addCollection(brStructCollection& col) { m_testData.push_back(col); }
        

        
        void updateDataString() { m_testData.updateCachedString(); }
        void updateVarConfigString() { m_varConfig.updateCachedString();}
        std::string getVarConfigString() const { return m_varConfig.m_cachedString; }

        selectableElement getId(size_t id){ return m_IdMap[id];}

        MirClipboard clipboard;
        std::unordered_map<size_t, selectableElement> m_IdMap;
    private:
        brTyp m_testData;
        brVarConfigCollection m_varConfig;
        

    };
}
