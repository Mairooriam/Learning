#pragma once
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui_internal.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

#include "MirParser.h"
namespace Mir {

    class TypeTable;




    namespace UI{
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
            std::string toString() override { return value; }
            void Clear() override { value = "N/A"; }
            RenderResultFlags Render() override;
            void SetRenderType(RenderType type) { m_RenderType = type; }
            RenderType GetRenderType() const { return m_RenderType; }
            bool IsRenderType(RenderType type) { return m_RenderType == type; }
            
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

            TableColumnSettings(const std::string& columnName = "", 
                       StringValue::RenderType type = StringValue::RenderType::Input,
                       const std::vector<std::string>& columnSuggestions = {},
                       float columnWidth = 0.0f)
                : name(columnName), 
                  renderType(type), 
                  suggestions(columnSuggestions),
                  width(columnWidth) {}
            };
            
        class BaseTableSettings {
        public:
            std::string tableName;
            std::vector<TableColumnSettings> columns;
            ImGuiID m_tableLinkingID;
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

            BaseTableSettings& WithTableLinkingTo(const std::string& name){
                m_tableLinkingID = ImHashStr(name.c_str(),0);
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
            std::string m_tableComment;
            std::vector<std::string> m_columnNames;
            std::vector<StringValue::RenderType> m_columnRenderTypes;
            std::vector<std::vector<std::string>> m_columnSuggestions;
            std::vector<float> m_columnWidths;
            bool m_isOpen = false;  
            bool m_showContextMenuHeader = false;
            int m_activeContextMenuColumn = -1; 
            bool m_showColumnContextMenu = false;
            ImGuiID m_LinkingTableID;
        
        protected:
            std::vector<std::vector<StringValue>> m_columns;
        
        public:
            BaseTable() = default;
            BaseTable(const BaseTableSettings& settings);
            BaseTable(const BaseTable& other, bool preserveIDs = true);
            void initializeSettings(const BaseTableSettings& settings);
            virtual ~BaseTable() = default;
        
            // Column operations
            void SetColumnNames(const std::vector<std::string>& names);
            const std::vector<std::string>& GetColumnNames() const { return m_columnNames; };
            size_t GetColumnCount() const { return m_columnNames.size(); };
            void SetTableComment(const std::string& comment) { m_tableComment = comment; }
            const std::string& GetTableComment() const { return m_tableComment; }
            void SetTableName(const std::string& name) { m_tableName = name; }
            const std::string& GetTableName() const { return m_tableName; }

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
            ImGuiID getGlobalTableID() { return m_LinkingTableID; }
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

            const StringValue& GetCell(size_t rowIndex, size_t columnIndex) const {
                static StringValue empty; 
                if (rowIndex >= RowCount() || columnIndex >= m_columns.size()) {
                    return empty;
                }
                return m_columns[columnIndex][rowIndex];
            }
        };
        //--------------------------------------------------------------------------------------------------------------------------------------------------
        // BASE TABLE END
        //--------------------------------------------------------------------------------------------------------------------------------------------------
        class CsvTable : public BaseTable
        {
        private:
            /* data */
        public:
            CsvTable(/* args */);
            ~CsvTable();

            virtual void RenderContextMenuHeader();
        };
        
        
        //--------------------------------------------------------------------------------------------------------------------------------------------------
        // STRUCT TABLE START
        //--------------------------------------------------------------------------------------------------------------------------------------------------
        class StructTable : public BaseTable {
            public:
                StructTable(size_t id = 0)
                    : BaseTable(CreateStructSettings("New Struct", id)) {
                }

                StructTable(const Mir::StructDefinition& structDef, size_t id = 0)
                    : BaseTable(CreateStructSettings(structDef.name, id)) {

                        SetTableComment(structDef.comment);
                        
                        for (const auto& member : structDef.members) {
                            std::vector<std::string> row = {
                                member.name,       
                                member.type,       
                                member.value,      
                                member.comment     
                            };
                            AddRow(row);
                        }
                    }
                void InitializeFromStructDef(const Mir::StructDefinition& structDef) {
                    Clear(); 
                    SetTableName(structDef.name);
                    SetTableComment(structDef.comment);
                    
                    for (const auto& member : structDef.members) {
                        std::vector<std::string> row = {
                            member.name,       
                            member.type,       
                            member.value,      
                            member.comment     
                        };
                        AddRow(row);
                    }
                }
                
                Mir::StructDefinition ToStructDefinition() const {
                Mir::StructDefinition structDef;
                structDef.name = GetTableName();
                structDef.comment = GetTableComment();
                
                // Convert rows back to members
                for (size_t i = 0; i < RowCount(); i++) {
                    Mir::MemberDefinition member;
                    member.name = GetCell(i, 0).GetValue();
                    member.type = GetCell(i, 1).GetValue();
                    member.value = GetCell(i, 2).GetValue();
                    member.comment = GetCell(i, 3).GetValue();
                    structDef.members.push_back(member);
                }
                
                return structDef;
            }    
                    
            private:
                static BaseTableSettings CreateStructSettings(const std::string& name, size_t id) {
                    return BaseTableSettings(name)
                        .WithColumn("Name", StringValue::RenderType::Input)
                        .WithDropdownColumn("Type", {"BOOL", "INT", "UINT", "REAL", "USINT", "STRING"})
                        .WithColumn("Value", StringValue::RenderType::Input)
                        .WithColumn("Comment", StringValue::RenderType::Input)
                        .WithID(id)
                        .WithTableLinkingTo("structTable");
                }
            };
        //--------------------------------------------------------------------------------------------------------------------------------------------------
        // STRUCT TABLE END
        //--------------------------------------------------------------------------------------------------------------------------------------------------

        //--------------------------------------------------------------------------------------------------------------------------------------------------
        // TYPE TABLE START
        //--------------------------------------------------------------------------------------------------------------------------------------------------
        class TypeTable : public IbrBase {
        private:
            std::string m_name;
            std::string m_comment;
            std::vector<std::unique_ptr<StructTable>> m_structTables;
            bool m_isOpen = true;
        
        public:
        TypeTable(const TypeDefinition& typeDef, size_t id = 0)
        : IbrBase(id), m_comment(typeDef.comment) {
        
            m_name = "Type" + std::to_string(GetID());
        for (const auto& structDef : typeDef.structs) {
            m_structTables.push_back(std::make_unique<StructTable>(structDef));
            }
        }
            
            ~TypeTable() {
                Clear();
            }
            TypeTable(TypeTable&& other) noexcept
                : IbrBase(other.GetID()),
                m_name(std::move(other.m_name)),
                m_comment(std::move(other.m_comment)),
                m_structTables(std::move(other.m_structTables)),
                m_isOpen(other.m_isOpen) {}

            TypeTable& operator=(TypeTable&& other) noexcept {
                if (this != &other) {
                    m_name = std::move(other.m_name);
                    m_comment = std::move(other.m_comment);
                    m_structTables = std::move(other.m_structTables);
                    m_isOpen = other.m_isOpen;
                    SetID(other.GetID());
                }
                return *this;
            }
            void AddStructTable(std::unique_ptr<StructTable> table) {
                if (table) {
                    m_structTables.push_back(std::move(table));
                }
            }
            
            StructTable* GetStructTable(size_t index) {
                if (index < m_structTables.size()) {
                    return m_structTables[index].get();
                }
                return nullptr;
            }
            
            size_t GetStructTableCount() const {
                return m_structTables.size();
            }
            
            const std::string& GetComment() const { return m_comment; }
            void SetComment(const std::string& comment) { m_comment = comment; }
            
            const std::string& GetName() const { return m_name; }
            void SetName(const std::string& name) { m_name = name; }
            
            // Convert back to a TypeDefinition
            TypeDefinition ToTypeDefinition() const {
                TypeDefinition typeDef;
                typeDef.comment = m_comment;
                
                for (const auto& structTable : m_structTables) {
                    if (structTable) {
                        typeDef.structs.push_back(structTable->ToStructDefinition());
                    }
                }
                
                return typeDef;
            }
            
            // IbrBase implementation
            std::string toString() override {
                return m_name;
            }
            
            void Clear() override {
                m_structTables.clear();
            }
            
            RenderResultFlags Render() override {
                RenderResultFlags result = RenderResultFlags::None;
                
                std::string typLabel = m_name + "\t\t\t" + m_comment;
                if (ImGui::TreeNode(typLabel.c_str())) {
                    // Render each struct table
                    for (const auto& table : m_structTables) {
                        if (table) {
                            RenderResultFlags tableResult = table->Render();
                            result |= tableResult;
                        }
                    }
                    
                    ImGui::TreePop();
                }
                
                return result;
            }
        };
        //--------------------------------------------------------------------------------------------------------------------------------------------------
        // TYPE TABLE END
        //--------------------------------------------------------------------------------------------------------------------------------------------------    
        class IomTable : public BaseTable {
        private:
            bool m_isOpen = true;
        
        public:
            IomTable(const VarConfigDefinition& varConfig, size_t id = 0)
            : BaseTable(CreateSettings(varConfig.comment, id)) {

                SetTableComment(varConfig.comment);

                for (auto member : varConfig.members)
                {
                    std::vector<std::string> row = {
                        member.ioAdress,      
                        member.processVar,       
                        member.comment,          
                    };
                    AddRow(row);
                }
                
             
             
            }
            
                
            ~IomTable() {
                Clear();
            }
            
            // RenderResultFlags Render() override {
            //     RenderResultFlags result = RenderResultFlags::None;

            // }

            private:
            static BaseTableSettings CreateSettings(const std::string& name, size_t id) {
                return BaseTableSettings(name)
                    .WithColumn("IO Adress", StringValue::RenderType::Input)
                    .WithColumn("Process Variable", StringValue::RenderType::Input)
                    .WithColumn("Comment", StringValue::RenderType::Input)
                    .WithID(id)
                    .WithTableLinkingTo("iomTable");;
                }
            };
        //--------------------------------------------------------------------------------------------------------------------------------------------------
        // VAR CONFIG MEMEBER START
        //--------------------------------------------------------------------------------------------------------------------------------------------------
        
        //--------------------------------------------------------------------------------------------------------------------------------------------------
        // VAR CONFIG MEMEBER END
        //--------------------------------------------------------------------------------------------------------------------------------------------------
        
        
        //--------------------------------------------------------------------------------------------------------------------------------------------------
        // VAR CONFIG TABLE START
        //--------------------------------------------------------------------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------------------------------------------------------------------
        // VAR CONFIG TABLE END
        //--------------------------------------------------------------------------------------------------------------------------------------------------


        //--------------------------------------------------------------------------------------------------------------------------------------------------
        // UTILS START
        //--------------------------------------------------------------------------------------------------------------------------------------------------
        namespace Utils {
            // Convert Parser result to UI components
            inline std::vector<TypeTable*> CreateTypeTablesFromParseResult(const Parser::ParseResult& result) {
                std::vector<TypeTable*> tables;
                
                for (const auto& typeDef : result.typDefinitions) {
                    tables.push_back(new TypeTable(typeDef));
                }
                
                return tables;
            }
            
            // Convert UI components back to Parser result
            inline Parser::ParseResult CreateParseResultFromTypeTables(const std::vector<UI::TypeTable*>& tables) {
                Parser::ParseResult result;
                
                for (const auto* table : tables) {
                    if (table) {
                        result.typDefinitions.push_back(table->ToTypeDefinition());
                    }
                }
                
                return result;
            }
        }
        //--------------------------------------------------------------------------------------------------------------------------------------------------
        // UTILS END
        //--------------------------------------------------------------------------------------------------------------------------------------------------

}
}