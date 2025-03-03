#pragma once

#include <string>
#include <vector>
#include <ios>

#include <map>
#include "brTypes.h"



namespace Mir {

    struct MirClipboard {
        enum class ContentType {
            None,
            String,
            BrCollection,
            BrStruct
        };
        
        std::string stringContent;
        
        // Add these fields to store the actual copied data
        brStructCollection collectionData;
        brStructNode structData;
        
        // Indices for the copied item (if needed for reference)
        size_t collectionIndex = 0;
        size_t structIndex = 0;
        
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
        
        bool isEmpty() const {
            return contentType == ContentType::None;
        }
        
        // Existing methods...
        
        // Add these methods for copying
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
        
        // Improved toString() implementation
        std::string toString() const {
            if (contentType == ContentType::String) {
                return stringContent;
            } else if (contentType == ContentType::BrCollection) {
                return collectionData.toString();
            } else if (contentType == ContentType::BrStruct) {
                return structData.toString();
            }
            return "";
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
    };

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
        
        MirClipboard clipboard;
    private:
        brTyp m_testData;
        

    };
}
