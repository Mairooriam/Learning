#pragma once
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
namespace Mir {


    class IDGenerator {
        private:
            static IDGenerator* s_Instance;
            size_t m_CurrentID;
    
            IDGenerator() : m_CurrentID(1) {}
    
            IDGenerator(const IDGenerator&) = delete;
            IDGenerator& operator=(const IDGenerator&) = delete;
    
        public:
            // Get singleton instance
            static IDGenerator& GetInstance() {
                if (s_Instance == nullptr) {
                    s_Instance = new IDGenerator();
                }
                return *s_Instance;
            }
    
            // Generate a new unique ID
            size_t GenerateID() {
                return m_CurrentID++;
            }
    };

    class IbrBase
    {
    private:
        size_t m_id;
        bool m_IsDirty;
    protected:
        void SetID(size_t id) { m_id = id; }
    public:
        virtual std::string toString() = 0;
        virtual void Clear() = 0;
        size_t GetID() const { return m_id; }
        bool IsDirty() { return m_IsDirty; }
        void SetDirty(bool dirty) { m_IsDirty = dirty; }
        virtual void Render() = 0; 

        IbrBase(size_t id = 0) 
        : m_id(id == 0 ? IDGenerator::GetInstance().GenerateID() : id), 
          m_IsDirty(false) {}
        virtual ~IbrBase() { }
    };


    // class StringValue : public IbrBase
    // {
    // public:
    //     enum class RenderType
    //     { 
    //         Default, 
    //         MultiLine, 
    //         Input,
    //         Dropdown
    //     };


    //     StringValue(size_t id = 0);
    //     StringValue(const std::string& initialValue, RenderType type = RenderType::Default, size_t id = 0);
    //     ~StringValue();

    //     void SetValue(const std::string& val) { value = val; }
    //     std::string GetValue() { return value; }
    //     std::string GetValue() const { return value; }

    //     void SetRenderType(RenderType type) { m_RenderType = type; }
    //     RenderType GetRenderType() const { return m_RenderType; }

    //     std::string toString() override { return value; }

    //     void Clear() override { value = "N/A"; }
    //     void Render() override;

    // private:
    //     std::string value = "N/A";
    //     RenderType m_RenderType = RenderType::Default;

    // };
    
    // Add DropdownData as a new member in StringValue
    class StringValue : public IbrBase
    {
    public:
        enum class RenderType
        { 
            Default,
            MultiLine,
            Input,
            Dropdown  // Changed from DropDownInputText
        };

        StringValue(size_t id = 0);
        StringValue(const std::string& initialValue, RenderType type = RenderType::Default, size_t id = 0);
        ~StringValue();

        
        void SetValue(const std::string& val) { value = val; }
        std::string GetValue() { return value; }
        std::string GetValue() const { return value; }
        static StringValue CreateCopy(const StringValue& other);

        void SetRenderType(RenderType type) { m_RenderType = type; }
        RenderType GetRenderType() const { return m_RenderType; }
        bool IsRenderType(RenderType type) { return m_RenderType == type; }

        std::string toString() override { return value; }

        void Clear() override { value = "N/A"; }
        void Render() override;

        void SetSuggestions(const std::vector<std::string>& items) {
            m_DropdownItems = items;
            //m_RenderType = RenderType::Dropdown;
        }

        operator std::string() const { 
            return value;
        }

        //TODO: Is this even needed?
        const std::vector<std::string>& GetDropdownItems() const { return m_DropdownItems; }

    private:
        std::string value = "N/A";
        RenderType m_RenderType = RenderType::Default;
        
        std::vector<std::string> m_DropdownItems;
        struct {
            char searchBuffer[64] = {};
            bool focused = false;
            int selectedIndex = -1;
            bool autoCompleted = false;
        } m_DropdownState;
    };



    // class FieldSuggestionProvider {
    //     private:
    //         static FieldSuggestionProvider* s_Instance;
    //         std::unordered_map<std::string, std::vector<std::string>> m_SuggestionsByCategory;
    //         FieldSuggestionProvider() = default;
            
    //     public:
    //         static FieldSuggestionProvider& GetInstance() {
    //             if (s_Instance == nullptr) {
    //                 s_Instance = new FieldSuggestionProvider();
    //             }
    //             return *s_Instance;
    //         }
            
    //         const std::vector<std::string>& GetSuggestions(const std::string& category) const {
    //             static std::vector<std::string> empty;
    //             auto it = m_SuggestionsByCategory.find(category);
    //             return it != m_SuggestionsByCategory.end() ? it->second : empty;
    //         }
            
    //         void SetSuggestions(const std::string& category, const std::vector<std::string>& suggestions) {
    //             m_SuggestionsByCategory[category] = suggestions;
    //         }
            
    //         void AddSuggestion(const std::string& category, const std::string& suggestion) {
    //             auto& suggestions = m_SuggestionsByCategory[category];
    //             if (std::find(suggestions.begin(), suggestions.end(), suggestion) == suggestions.end()) {
    //                 suggestions.push_back(suggestion);
    //             }
    //         }

    //         std::vector<std::string> GetCategories() const {
    //             std::vector<std::string> categories;
    //             for (const auto& [category, suggestions] : m_SuggestionsByCategory) {
    //                 categories.push_back(category);
    //             }
    //             return categories;
    //         }

            
    //     };
    class TableColumnSettings {
        public:
            std::string name;
            StringValue::RenderType renderType = StringValue::RenderType::Input;
            std::vector<std::string> suggestions;
            float width = 0.0f; // 0 means auto/stretch
        };
        
    class BaseTableSettings {
    public:
        std::string tableName;
        std::vector<TableColumnSettings> columns;
        size_t id = 0;
    
        // Constructor for convenience
        BaseTableSettings(const std::string& name = "Table") : tableName(name) {}
    
        // Fluent interface to build settings
        BaseTableSettings& WithColumn(const std::string& name, 
                                        StringValue::RenderType type = StringValue::RenderType::Input,
                                        const std::vector<std::string>& suggestions = {}) {
            columns.push_back({name, type, suggestions});
            return *this;
        }
        
        BaseTableSettings& WithDropdownColumn(const std::string& name, 
                                                const std::vector<std::string>& suggestions) {
            columns.push_back({name, StringValue::RenderType::Dropdown, suggestions});
            return *this;
        }
        
        BaseTableSettings& WithID(size_t tableID) {
            id = tableID;
            return *this;
        }
    };


    class BaseTable : public IbrBase {
        private:
        std::string m_tableName;
        std::vector<std::string> m_columnNames;
        std::vector<StringValue::RenderType> m_columnRenderTypes;
        std::vector<std::vector<std::string>> m_columnSuggestions;
        std::vector<float> m_columnWidths;
        bool m_isOpen = false;  
         
        protected:
        std::vector<std::vector<StringValue>> m_columns;
        
        public:
        BaseTable(const BaseTableSettings& settings);
            virtual ~BaseTable() = default;
        
            // Column operations
            void SetColumnNames(const std::vector<std::string>& names);
            const std::vector<std::string>& GetColumnNames() const { return m_columnNames; };
            size_t GetColumnCount() const { return m_columnNames.size(); };
            
            // Row operations
            virtual void AddRow(const std::vector<std::string>& values);
            virtual void RemoveRow(size_t rowIndex);
            size_t RowCount() const;
            
            // Cell operations
            StringValue& GetCell(size_t rowIndex, size_t columnIndex);
            void SetCellValue(size_t rowIndex, size_t columnIndex, const std::string& value);
            
            // Utility methods
            std::vector<StringValue> CopyRow(size_t rowIndex);
            void PasteRow(size_t rowIndex, const std::vector<StringValue>& values);
            
            // IbrBase requirements
            void Clear() override;
            void Render() override;
            std::vector<size_t> GetChildIDs() const;
            std::string toString() override;
        };
        
    
    class StructTable : public BaseTable {
        public:
            StructTable(const std::string& name, size_t id = 0)
                : BaseTable(CreateStructSettings(name, id)) {}
                
        private:
            static BaseTableSettings CreateStructSettings(const std::string& name, size_t id) {
                return BaseTableSettings(name)
                    .WithColumn("Name", StringValue::RenderType::Input)
                    .WithDropdownColumn("Type", {"int", "float", "bool", "string", "double"})
                    .WithColumn("Value", StringValue::RenderType::Input)
                    .WithColumn("Comment", StringValue::RenderType::Input)
                    .WithID(id);
            }
        };
    // class brStruct : public IbrBase
    // {
    // private:
    //     std::vector<StringValue> m_fields[4]; 
    //     std::vector<std::string> m_fieldNames;
    //     std::string m_structName;
    //     size_t m_size = 0;
        
        
    //     size_t FieldsSize() const { return m_fields[0].size(); }
    // public:
    //     brStruct(size_t id = 0);
    //     brStruct(const std::string& structName, const std::vector<std::string>& fieldNames, size_t id = 0);
    //     ~brStruct();
        
    //     void SetFieldNames(const std::vector<std::string>& fieldNames);
    //     const std::vector<std::string>& GetFieldNames() const { return m_fieldNames; }
    //     StringValue& GetField(size_t rowIndex, size_t fieldIndex) { return m_fields[fieldIndex][rowIndex]; }
    //     StringValue& GetField(size_t rowIndex, const std::string& fieldName);
        
    //     void SetFieldValue(size_t rowIndex, size_t fieldIndex, const std::string& value);
    //     void SetFieldValue(size_t rowIndex, const std::string& fieldName, const std::string& value);



    //     std::string GetStructName() { return m_structName; }
    //     std::vector<StringValue> copyAtIndex(size_t i);
    //     void pasteToIndex(size_t i, const std::vector<StringValue>& values);
        
    //     void push_back(const std::string& _name, const std::string& _type, const std::string& _value, const std::string& _comment);
    //     void deleteAt(size_t i);
        
    //     void SetTypeSuggestions(const std::vector<std::string>& suggestions);

    //     std::vector<size_t> GetChildIDs() const {
    //         std::vector<size_t> ids;
    //         for (auto &&field : m_fields) {
    //             for (const auto& value : field) {
    //                 ids.push_back(value.GetID());
    //             }
    //         }
    //         return ids;
    //     }
        
    //     std::vector<size_t> GetAllIds() const {
    //         std::vector<size_t> result = {GetID()};
    //         std::vector<size_t> childIds = GetChildIDs();
    //         result.insert(result.end(), childIds.begin(), childIds.end());
    //         return result;
    //     }

    //     std::string GetAllIDsAsString() const {
    //         std::vector<size_t> ids = GetAllIds();
    //         std::string result;
    //         for (size_t i = 0; i < ids.size(); i++) {
    //             result += std::to_string(ids[i]);
    //             if (i < ids.size() - 1) {
    //                 result += ", ";
    //             }
    //         }
    //         return result;
    //     }

    //     std::string toString() override;
    //     void Clear() override;
    //     void Render() override;
    // };



    


    // class brVarConfigData : public IbrData
    // {
    // private:
    //     std::string m_ioAdress = "No ioAdress";                 
    //     std::string m_processVariable = "No ProcessVariable";
    //     std::string m_type = "No Type";
    //     std::string m_comment = "No Comment";
       
    // public:
    //     brVarConfigData();
    //     brVarConfigData(const std::string& ioAdress, const std::string& processVariable, 
    //             const std::string& type, const std::string& comment);
    //     ~brVarConfigData();
    
    //     std::string toString() override; 
    //     void Clear() override;    

    // };
    

    
}