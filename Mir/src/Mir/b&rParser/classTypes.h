#pragma once
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
namespace Mir {

    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // RENDER RESULT FLAGS START
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    enum class RenderResultFlags {
        None        = 0,
        ValueChanged = 1 << 0,  
        StructureChanged = 1 << 1,  
        VisibilityChanged = 1 << 2,  
        SelectionChanged = 1 << 3,  
        DeleteRequested = 1 << 4,  
        DragDropped = 1 << 5,  
        ContextMenuOpened = 1 << 6, 
        
        AnyChange = ValueChanged | StructureChanged | VisibilityChanged | SelectionChanged,
        DataModified = ValueChanged | StructureChanged
    };

    inline RenderResultFlags operator|(RenderResultFlags a, RenderResultFlags b) {
        return static_cast<RenderResultFlags>(static_cast<int>(a) | static_cast<int>(b));
    }

    inline RenderResultFlags& operator|=(RenderResultFlags& a, RenderResultFlags b) {
        return a = a | b;
    }

    inline RenderResultFlags operator&(RenderResultFlags a, RenderResultFlags b) {
        return static_cast<RenderResultFlags>(static_cast<int>(a) & static_cast<int>(b));
    }

    inline RenderResultFlags& operator&=(RenderResultFlags& a, RenderResultFlags b) {
        return a = a & b;
    }

    inline RenderResultFlags operator~(RenderResultFlags a) {
        return static_cast<RenderResultFlags>(~static_cast<int>(a));
    }

    // Helper function to check if flag is set
    inline bool HasFlag(RenderResultFlags flags, RenderResultFlags flag) {
        return (static_cast<int>(flags) & static_cast<int>(flag)) == static_cast<int>(flag);
    }
    namespace Utils{
        inline std::string RenderResultFlagsToString(RenderResultFlags flags) {
            if (flags == RenderResultFlags::None) return "None";
            
            std::string result;
            if ((flags & RenderResultFlags::ValueChanged) != RenderResultFlags::None) result += "ValueChanged|";
            if ((flags & RenderResultFlags::StructureChanged) != RenderResultFlags::None) result += "StructureChanged|";
            if ((flags & RenderResultFlags::VisibilityChanged) != RenderResultFlags::None) result += "VisibilityChanged|";
            if ((flags & RenderResultFlags::SelectionChanged) != RenderResultFlags::None) result += "SelectionChanged|";
            if ((flags & RenderResultFlags::DeleteRequested) != RenderResultFlags::None) result += "DeleteRequested|";
            if ((flags & RenderResultFlags::DragDropped) != RenderResultFlags::None) result += "DragDropped|";
            if ((flags & RenderResultFlags::ContextMenuOpened) != RenderResultFlags::None) result += "ContextMenuOpened|";
            
            // Remove trailing separator if present
            if (!result.empty()) {
                result.pop_back();
            }
            
            return result;
        }
    }
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // RENDER RESULT FLAGS END
    //--------------------------------------------------------------------------------------------------------------------------------------------------


    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // ID GENERATOR START
    //--------------------------------------------------------------------------------------------------------------------------------------------------
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
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // ID GENERATOR END
    //--------------------------------------------------------------------------------------------------------------------------------------------------


    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // IbrBase START
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    class IbrBase
    {
    private:
        size_t m_id;
        bool m_Dirty;
    protected:
        void SetID(size_t id) { m_id = id; }
    public:
        virtual std::string toString() = 0;
        virtual void Clear() = 0;
        size_t GetID() const { return m_id; }
        bool IsDirty() { return m_Dirty; }
        void SetDirty(bool dirty) { m_Dirty = dirty; }
        virtual void ClearDirty() { m_Dirty = false; }
        virtual RenderResultFlags Render() = 0; 

        IbrBase(size_t id = 0) 
        : m_id(id == 0 ? IDGenerator::GetInstance().GenerateID() : id), 
            m_Dirty(false) {}
        virtual ~IbrBase() { }
    };
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // IbrBase END
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    

    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // STRING VALUE START
    //--------------------------------------------------------------------------------------------------------------------------------------------------
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
        RenderResultFlags Render() override;

        void SetSuggestions(const std::vector<std::string>& items) {
            m_DropdownItems = items;
            //m_RenderType = RenderType::Dropdown;
        }

        operator std::string() const { 
            return value;
        }
        static StringValue CreateCopy(const StringValue& other, bool preserveID = false);
        
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
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // STRING VALUE END
    //--------------------------------------------------------------------------------------------------------------------------------------------------




    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // TABLE SETTINGS START
    //--------------------------------------------------------------------------------------------------------------------------------------------------
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
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // TABLE SETTINGS END
    //--------------------------------------------------------------------------------------------------------------------------------------------------


    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // BASE TABLE START
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    class BaseTable : public IbrBase {
    private:
        std::string m_tableName;
        std::vector<std::string> m_columnNames;
        std::vector<StringValue::RenderType> m_columnRenderTypes;
        std::vector<std::vector<std::string>> m_columnSuggestions;
        std::vector<float> m_columnWidths;
        bool m_isOpen = false;  
        bool m_showContextMenuHeader = false;
        int m_activeContextMenuColumn = -1; 
        bool m_showColumnContextMenu = false;
    
    protected:
        std::vector<std::vector<StringValue>> m_columns;
    
    public:
        BaseTable(const BaseTableSettings& settings);
        BaseTable(const BaseTable& other, bool preserveIDs = true);
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
        
        RenderResultFlags Render() override;
        virtual void RenderContextMenuHeader();
        virtual void RenderContextMenuColumns(int column);
        void Clear() override;
        std::vector<size_t> GetChildIDs() const;
        std::string toString() override;
        void SortByColumn(int column, bool ascending);

        virtual BaseTable* Clone() const {
            return new BaseTable(*this);  // Uses copy constructor
        }
    };
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // BASE TABLE END
    //--------------------------------------------------------------------------------------------------------------------------------------------------

        
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // STRUCT TABLE START
    //--------------------------------------------------------------------------------------------------------------------------------------------------
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
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // STRUCT TABLE END
    //--------------------------------------------------------------------------------------------------------------------------------------------------

    

    
}