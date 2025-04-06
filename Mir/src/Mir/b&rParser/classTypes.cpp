#include "classTypes.h"

namespace Mir {
    Mir::IDGenerator* Mir::IDGenerator::s_Instance = nullptr;


    
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // STRING VALUE START
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    StringValue::StringValue(size_t id)
        : IbrBase(id)
    {
        // Default constructor - value is already initialized to "N/A" in the class declaration
    }
    
    StringValue::StringValue(const std::string& initialValue, RenderType type, size_t id)
    : IbrBase(id), value(initialValue), m_RenderType(type)
    {
        
    }
    
    StringValue::~StringValue(){ }
    
    StringValue StringValue::CreateCopy(const StringValue& other) {
        // Pass 0 to force generation of a new ID
        StringValue copy(other.GetValue(), other.GetRenderType(), 0);
        
        if (other.GetRenderType() == RenderType::Dropdown) {
            copy.SetSuggestions(other.GetDropdownItems());
        }
        return copy;
    }

    void StringValue::Render()
    {
        ImGui::PushID(static_cast<int>(GetID()));
        
        bool changed = false;

        
        if (IsRenderType(RenderType::Default))
        {
            ImGui::TextUnformatted(value.c_str());
        } else if (IsRenderType(RenderType::MultiLine))
        {
            ImGui::SetNextItemWidth(300.0f);
            changed = ImGui::InputTextMultiline("", &value, ImVec2(0, 100.0f));
        } else if (IsRenderType(RenderType::Input))
        {
            ImGui::SetNextItemWidth(200.0f);
            changed = ImGui::InputText("", &value);
        } else if (IsRenderType(RenderType::Dropdown))
        {
            ImGui::SetNextItemWidth(200.0f);
                
                if (m_DropdownItems.empty()) {
                    //ImGui::TextUnformatted("No dropdown items"); 
                    MIR_ASSERT(false,"Set suggestions for field before calling render on Dropdown");
                }
                
                // Store local references to dropdown state for clarity
                auto& searchBuffer = m_DropdownState.searchBuffer;
                auto& focused = m_DropdownState.focused;
                auto& selectedIndex = m_DropdownState.selectedIndex;
                auto& autoCompleted = m_DropdownState.autoCompleted;
                
                // Begin the combo dropdown
                if (ImGui::BeginCombo("", value.c_str(), ImGuiComboFlags_HeightLarge)) {
                    // Handle focus when opening dropdown
                    if (!focused) {
                        ImGui::SetKeyboardFocusHere();
                        focused = true;
                        strncpy(searchBuffer, value.c_str(), sizeof(searchBuffer) - 1);
                        searchBuffer[sizeof(searchBuffer) - 1] = '\0';
                        selectedIndex = -1;
                        autoCompleted = false;
                    }
                    
                    // Search input at top of dropdown
                    ImGui::PushItemWidth(-1);
                    if (ImGui::InputText("##search", searchBuffer, sizeof(searchBuffer),
                        ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
                        value = searchBuffer;
                        changed = true;
                        focused = false;
                        ImGui::CloseCurrentPopup();
                    }
                    
                    // Tab key for autocomplete
                    if (ImGui::IsKeyPressed(ImGuiKey_Tab) && !autoCompleted) {
                        std::string_view search(searchBuffer);
                        std::vector<size_t> visibleItems;
                        
                        for (size_t idx = 0; idx < m_DropdownItems.size(); idx++) {
                            if (search.empty() || 
                                m_DropdownItems[idx].find(search) != std::string_view::npos) {
                                visibleItems.push_back(idx);
                            }
                        }
                        
                        if (!visibleItems.empty()) {
                            selectedIndex = visibleItems[0];
                            strncpy(searchBuffer, m_DropdownItems[selectedIndex].c_str(), 
                                    sizeof(searchBuffer) - 1);
                            searchBuffer[sizeof(searchBuffer) - 1] = '\0';
                            autoCompleted = true;
                        }
                    }
                    
                    ImGui::PopItemWidth();
                    ImGui::Separator();
                    
                    // Show filtered dropdown items
                    std::string_view search(searchBuffer);
                    bool hasVisibleItems = false;
                    
                    for (size_t idx = 0; idx < m_DropdownItems.size(); idx++) {
                        const auto& item = m_DropdownItems[idx];
                        
                        // Case-insensitive search
                        bool match = search.empty();
                        if (!match) {
                            std::string lowerSearch(search);
                            std::string lowerItem(item);
                            
                            std::transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(),
                                [](unsigned char c){ return std::tolower(c); });
                            std::transform(lowerItem.begin(), lowerItem.end(), lowerItem.begin(),
                                [](unsigned char c){ return std::tolower(c); });
                            
                            match = lowerItem.find(lowerSearch) != std::string::npos;
                        }
                        
                        if (match) {
                            const bool is_selected = (selectedIndex == static_cast<int>(idx));
                            
                            if (ImGui::Selectable(item.c_str(), is_selected)) {
                                value = item;
                                changed = true;
                                ImGui::CloseCurrentPopup();
                            }
                            
                            if (is_selected) {
                                ImGui::SetItemDefaultFocus();
                            }
                            
                            hasVisibleItems = true;
                        }
                    }
                    
                    ImGui::EndCombo();
                } 
                else if (focused) {
                    focused = false;
                }
            
        }

        
        ImGui::PopID();

        if (changed) {
            SetDirty(true);
        }
    }
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // STRING VALUE END
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    BaseTable::BaseTable(const BaseTableSettings& settings)
    : IbrBase(settings.id), m_tableName(settings.tableName)
    {
        size_t columnCount = settings.columns.size();
        
        m_columnNames.resize(columnCount);
        m_columnRenderTypes.resize(columnCount);
        m_columnSuggestions.resize(columnCount);
        m_columnWidths.resize(columnCount);
        m_columns.resize(columnCount);
        
        for (size_t i = 0; i < columnCount; i++) {
            m_columnNames[i] = settings.columns[i].name;
            m_columnRenderTypes[i] = settings.columns[i].renderType;
            m_columnSuggestions[i] = settings.columns[i].suggestions;
            m_columnWidths[i] = settings.columns[i].width;
        }
    }
    
    void BaseTable::SetColumnNames(const std::vector<std::string> &names)
    {
        if (names.size() == m_columnNames.size() )
        {
            m_columnNames = names;
        } else {
            MIR_ASSERT(false, "Column names count does not match the table's column count");
        }
        
    }

    void BaseTable::AddRow(const std::vector<std::string> &values){
        if (values.size() != m_columns.size()) {
            MIR_ASSERT(false, "Add row failed. Supplied values are not the size of table (columns)");
            return;
        }
        
        for (size_t i = 0; i < m_columns.size(); i++) {
            m_columns[i].emplace_back(values[i], m_columnRenderTypes[i], 0);
            
            // Apply suggestions if column is a dropdown
            if (m_columnRenderTypes[i] == StringValue::RenderType::Dropdown && 
                !m_columnSuggestions[i].empty()) {
                m_columns[i].back().SetSuggestions(m_columnSuggestions[i]);
            }
        }
    }

    void BaseTable::RemoveRow(size_t rowIndex)
    {
        if (rowIndex >= RowCount()) {
            return;
        }
        
        for (auto& column : m_columns) {
            column.erase(column.begin() + rowIndex);
        }
    }

    void BaseTable::Render()
    {
       bool changed = false;
        
        ImGui::PushID(static_cast<int>(GetID()));
        bool headerOpen = ImGui::CollapsingHeader(m_tableName.c_str());
        ImGui::PopID();
        
        if (headerOpen && !m_columnNames.empty()) {
            if (ImGui::BeginTable("TableData", m_columnNames.size())) {
                // Setup columns
                for (size_t i = 0; i < m_columnNames.size(); i++) {
                    ImGuiTableColumnFlags flags = m_columnWidths[i] > 0 ? 
                        ImGuiTableColumnFlags_WidthFixed : ImGuiTableColumnFlags_WidthStretch;
                        
                    ImGui::TableSetupColumn(m_columnNames[i].c_str(), flags, m_columnWidths[i]);
                }
                ImGui::TableHeadersRow();
                
                // Render rows
                size_t rowCount = RowCount();
                for (size_t rowIdx = 0; rowIdx < rowCount; rowIdx++) {
                    ImGui::TableNextRow();
                    
                    // Data cells
                    for (size_t colIdx = 0; colIdx < m_columns.size(); colIdx++) {
                        ImGui::TableNextColumn();
                        ImGui::PushID(static_cast<int>(m_columns[colIdx][rowIdx].GetID()));
                        
                        m_columns[colIdx][rowIdx].Render();
                        if (m_columns[colIdx][rowIdx].IsDirty()) {
                            m_columns[colIdx][rowIdx].SetDirty(false);
                            changed = true;
                        }
                        
                        ImGui::PopID();
                    }
                }
                
                ImGui::EndTable();
            }
        }
        
        if (changed) {
            SetDirty(true);
        }
    }

    StringValue& BaseTable::GetCell(size_t rowIndex, size_t columnIndex)
    {
        return m_columns[columnIndex][rowIndex];
    }

    void BaseTable::SetCellValue(size_t rowIndex, size_t columnIndex, const std::string& value)
    {
        if (rowIndex < RowCount() && columnIndex < m_columns.size()) {
            m_columns[columnIndex][rowIndex].SetValue(value);
        }
    }

    std::vector<StringValue> BaseTable::CopyRow(size_t rowIndex)
    {
        std::vector<StringValue> result;
        if (rowIndex >= RowCount()) return result;
        
        for (auto& column : m_columns) {
            result.push_back(column[rowIndex]);
        }
        return result;
    }

    void BaseTable::PasteRow(size_t rowIndex, const std::vector<StringValue>& values)
    {
        if (values.size() != m_columns.size()) return;
        if (rowIndex > RowCount()) rowIndex = RowCount();
        
        for (size_t i = 0; i < m_columns.size(); i++) {
            m_columns[i].insert(m_columns[i].begin() + rowIndex, 
                            StringValue::CreateCopy(values[i]));
        }
    }

    std::vector<size_t> BaseTable::GetChildIDs() const
    {
        std::vector<size_t> ids;
        for (const auto& column : m_columns) {
            for (const auto& cell : column) {
                ids.push_back(cell.GetID());
            }
        }
        return ids;
    }

    size_t BaseTable::RowCount() const
    {
        return m_columns.empty() ? 0 : m_columns[0].size();
    }

    void BaseTable::Clear()
    {
        for (auto& column : m_columns) {
            column.clear();
        }
    }
    
    std::string BaseTable::toString()
    {
        MIR_ASSERT(false, "not implemented");
        return "PLACEHOLDER";
    }
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // BASE TABLE START
    //--------------------------------------------------------------------------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // BASE TABLE END
    //--------------------------------------------------------------------------------------------------------------------------------------------------


    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // STRUCT START
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // brStruct::brStruct(size_t id)
    // : IbrBase(id) 
    // {
    // }

    // brStruct::brStruct(const std::string &structName, const std::vector<std::string> &fieldNames, size_t id)
    // : IbrBase(id),
    // m_structName(name),
    // m_fieldNames(fieldNames);
    // {
    // }

    // brStruct::brStruct(const std::string& name, size_t id)
    //     : IbrBase(id),
    //     m_structName(name)
    // {
    // }

    // brStruct::~brStruct()
    // {
    // }

    // std::vector<StringValue> brStruct::copyAtIndex(size_t i)
    // {
    //     std::vector<StringValue> buf;
    //     if (i >= m_size && m_size > 0) {
    //         i = m_size - 1;
    //     }
        
    //     if (m_size > 0) {
    //         for (int field = 0; field < 4; field++) {
    //             buf.push_back(m_fields[field][i]);
    //         }
    //     }
    //     return buf;
    // }

    // void brStruct::pasteToIndex(size_t i, const std::vector<StringValue>& values)
    // {
    //     if (values.size() == 4) {
    //         if (i > m_size) {
    //             i = m_size;
    //         }
            
    //         for (int field = 0; field < 4; field++) {
    //             m_fields[field].insert(m_fields[field].begin() + i, 
    //                                   StringValue::CreateCopy(values[field]));
    //         }
            
    //         m_size++;
    //     }
    // }

    // void brStruct::push_back(const std::string &_name, const std::string &_type, const std::string &_value, const std::string &_comment)
    // {
    //     // Create objects with explicit ID = 0 to force new ID generation
    //     m_fields[0].emplace_back(_name, StringValue::RenderType::Input, 0);
    //     m_fields[1].emplace_back(_type, StringValue::RenderType::Dropdown, 0);
    //     m_fields[2].emplace_back(_value, StringValue::RenderType::Input, 0);
    //     m_fields[3].emplace_back(_comment, StringValue::RenderType::Input, 0);
        

    // }
    // void brStruct::deleteAt(size_t i)
    // {
    //     if (i < m_size)
    //     {
    //         for (int field = 0; field < 4; field++)
    //         {
    //             m_fields[field].erase(m_fields[field].begin() + i);
    //         }
    //         m_size--;
    //     }
    // }

    // void brStruct::SetTypeSuggestions(const std::vector<std::string> &suggestions)
    // {
    //     for (auto& type : m_fields[1])
    //     {
    //         type.SetSuggestions(suggestions);
    //     }
    // }

    // void brStruct::Render()
    // {
    //     bool changed = false;
    
    //     ImGui::PushID(static_cast<int>(GetID()));
    //     bool structOpen = ImGui::CollapsingHeader(m_structName.c_str());
    //     ImGui::PopID();
    //     if (structOpen)
    //     {
    //         if (ImGui::BeginTable("StructTable", 5))
    //         {
    //             ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
    //             ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed);
    //             ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed);
    //             ImGui::TableSetupColumn("Comment", ImGuiTableColumnFlags_WidthStretch);
    //             ImGui::TableHeadersRow();
                
    //             for (size_t i = 0; i < m_size; i++)
    //             {
    //                 ImGui::TableNextRow();
                    
    //                 for (int field = 0; field < 4; field++) {
    //                     // Column for this field
    //                     ImGui::TableSetColumnIndex(field);
    //                     ImGui::PushID(static_cast<int>(m_fields[field][i].GetID()));
                        
    //                     // For type field (field=1), check if suggestions are set
    //                     if (field == 1 && m_fields[field][i].GetDropdownItems().empty()) {
    //                         MIR_ASSERT(false, "Add suggestions for struct's type field before rendering using SetTypeSuggestions()");
    //                     }
                        
    //                     m_fields[field][i].Render();
                        
    //                     if (m_fields[field][i].IsDirty()) {
    //                         m_fields[field][i].SetDirty(false);
    //                         changed = true;
    //                     }
                        
    //                     ImGui::PopID();
    //                 }
    //             }
                
    //             if (changed) {
    //                 SetDirty(true);
    //             }
                
    //             ImGui::EndTable();
    //         }
    //     }
    // }
    // std::string brStruct::toString()
    // {
    //     std::stringstream ss;
    //     for (size_t i = 0; i < m_size; i++)
    //     {
    //         ss << m_fields[0][i].GetValue(); // Name
    //         if (!m_fields[1][i].GetValue().empty()) ss << " : " << m_fields[1][i].GetValue(); // Type
    //         if (!m_fields[2][i].GetValue().empty()) ss << " := " << m_fields[2][i].GetValue(); // Value
    //         if (!m_fields[3][i].GetValue().empty()) ss << "; (*" << m_fields[3][i].GetValue() << "*)"; // Comment
    //     }
    //     return ss.str();
    // }

    // void brStruct::Clear()
    // {
    //     for (size_t i = 0; i < elementCount(); i++)
    //     {
    //         m_names[i].Clear();
    //         m_types[i].Clear();
    //         m_values[i].Clear();
    //         m_comments[i].Clear();
    //     }
    // }
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // STRUCT END
    //--------------------------------------------------------------------------------------------------------------------------------------------------

   
}