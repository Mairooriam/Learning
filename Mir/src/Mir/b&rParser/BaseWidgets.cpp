#include "BaseWidgets.h"

namespace Mir {
    namespace UI {
    Mir::UI::IDGenerator* Mir::UI::IDGenerator::s_Instance = nullptr;


    
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
    
    StringValue StringValue::CreateCopy(const StringValue &other, bool preserveID)
    {
        StringValue copy(other.value, other.m_RenderType, 
        preserveID ? other.GetID() : 0);

        copy.m_DropdownItems = other.m_DropdownItems;
        return copy;
    }

    RenderResultFlags StringValue::Render()
    {

        RenderResultFlags result = RenderResultFlags::None;

        ImGui::PushID(static_cast<int>(GetID()));
        
        if (IsRenderType(RenderType::Default))
        {
            ImGui::TextUnformatted(value.c_str());
        } else if (IsRenderType(RenderType::MultiLine))
        {
            ImGui::SetNextItemWidth(300.0f);
            if (ImGui::InputTextMultiline("", &value, ImVec2(0, 100.0f))){
                result |= RenderResultFlags::ValueChanged;
            }
            
        } else if (IsRenderType(RenderType::Input))
        {
            ImGui::SetNextItemWidth(200.0f);
            if (ImGui::InputText("", &value)){
                result |= RenderResultFlags::ValueChanged;
            }
        } else if (IsRenderType(RenderType::Dropdown))
        {
            ImGui::SetNextItemWidth(200.0f);
                
                if (m_DropdownItems.empty()) {
                    MIR_ASSERT(false,"Set suggestions for field before calling render on Dropdown");
                }

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
                        result |= RenderResultFlags::ValueChanged;
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
                                result |= RenderResultFlags::ValueChanged;
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
        
        if (HasFlag(result, RenderResultFlags::ValueChanged)) {
            SetDirty(true);
        }
        
        return result;
    }

    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // STRING VALUE END
    //--------------------------------------------------------------------------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // BASE TABLE START
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    BaseTable::BaseTable(const BaseTableSettings& settings)
    : IbrBase(settings.id), m_tableName(settings.tableName), m_LinkingTableID(settings.m_tableLinkingID)
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

    BaseTable::BaseTable(const BaseTable &other, bool preserveIDs)
        : IbrBase(preserveIDs ? other.GetID() : IDGenerator::GetInstance().GenerateID()),
        m_tableName(other.m_tableName),
        m_columnNames(other.m_columnNames),
        m_columnRenderTypes(other.m_columnRenderTypes),
        m_columnSuggestions(other.m_columnSuggestions),
        m_columnWidths(other.m_columnWidths),
        m_isOpen(other.m_isOpen),
        m_showContextMenuHeader(other.m_showContextMenuHeader), 
        m_activeContextMenuColumn(other.m_activeContextMenuColumn),   
        m_showColumnContextMenu(other.m_showColumnContextMenu) 
    {
    // Deep copy all columns and their StringValues
    m_columns.resize(other.m_columns.size());
    for (size_t col = 0; col < other.m_columns.size(); col++) {
        m_columns[col].reserve(other.m_columns[col].size());
        
        for (const auto& cell : other.m_columns[col]) {
            StringValue copiedCell = StringValue::CreateCopy(cell, preserveIDs);
            m_columns[col].push_back(copiedCell);
        }
    }
    }

    void BaseTable::initializeSettings(const BaseTableSettings& settings) {
        m_tableName = settings.tableName;
        m_LinkingTableID = settings.m_tableLinkingID;
        
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

    RenderResultFlags BaseTable::Render()
    {
        RenderResultFlags result = RenderResultFlags::None;
        

        ImGui::PushID(static_cast<int>(GetID()));
        //bool wasOpen = m_isOpen;
        std::string label = m_tableName + "\t\t\t" + m_tableComment;
        bool headerOpen = ImGui::CollapsingHeader(label.c_str());
        
        // if (wasOpen != m_isOpen) {
        //     result |= RenderResultFlags::VisibilityChanged;
        // }
        
        ImGui::PopID();

        if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
            m_showContextMenuHeader = true;
            result |= RenderResultFlags::ContextMenuOpened;
        }
         static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoSavedSettings;
        if (headerOpen && !m_columnNames.empty()) {

            ImGui::PushOverrideID(m_LinkingTableID);
            if (ImGui::BeginTable("TableData", m_columnNames.size(), flags)) {
                // Setup columns
                for (size_t i = 0; i < m_columnNames.size(); i++) {
                    ImGuiTableColumnFlags flags = m_columnWidths[i] > 0 ? 
                        ImGuiTableColumnFlags_WidthFixed : ImGuiTableColumnFlags_WidthStretch;
                        
                    ImGui::TableSetupColumn(m_columnNames[i].c_str(), flags, m_columnWidths[i]);
                }
                ImGui::TableHeadersRow();

                for (int column = 0; column < (int)m_columnNames.size(); column++) {
                    if (ImGui::TableGetColumnFlags(column) & ImGuiTableColumnFlags_IsHovered &&
                        ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
                        m_activeContextMenuColumn = column;
                        m_showColumnContextMenu = true;
                    }
                }


                // Render rows
                size_t rowCount = RowCount();
                for (size_t rowIdx = 0; rowIdx < rowCount; rowIdx++) {
                    ImGui::TableNextRow();
                    
                    // Data cells
                    for (size_t colIdx = 0; colIdx < m_columns.size(); colIdx++) {
                        ImGui::TableNextColumn();
                        ImGui::PushID(static_cast<int>(m_columns[colIdx][rowIdx].GetID()));
                        
                        RenderResultFlags cellResult = m_columns[colIdx][rowIdx].Render();
                        
                        result |= cellResult;
                        
                        ImGui::PopID();
                    }
                }
                
                ImGui::EndTable();
            }
            ImGui::PopID();
        }
        
        // Show context menu if activated
        if (m_showContextMenuHeader) {
            ImGui::OpenPopup(("##TableContextMenu" + std::to_string(GetID())).c_str());
            m_showContextMenuHeader = false;
        }
        
        // Render the context menu
        if (ImGui::BeginPopup(("##TableContextMenu" + std::to_string(GetID())).c_str())) {
            RenderContextMenuHeader();
            ImGui::EndPopup();
        }

        // Show column context menu if activated
        if (m_showColumnContextMenu) {
            ImGui::OpenPopup(("##ColumnContextMenu" + std::to_string(GetID())).c_str());
            m_showColumnContextMenu = false;
        }
        
        // Render the column context menu
        if (ImGui::BeginPopup(("##ColumnContextMenu" + std::to_string(GetID())).c_str())) {
            RenderContextMenuColumns(m_activeContextMenuColumn);
            ImGui::EndPopup();
        }
        if (HasFlag(result, RenderResultFlags::DataModified)) {
            SetDirty(true);
        }

        return result;
    }

    void BaseTable::SortByColumn(int column, bool ascending) {
        // Create a vector of indices
        std::vector<size_t> indices(RowCount());
        for (size_t i = 0; i < indices.size(); i++) {
            indices[i] = i;
        }
        
        // Sort indices based on the column values
        std::sort(indices.begin(), indices.end(),
            [this, column, ascending](size_t a, size_t b) {
                const std::string& valA = this->GetCell(a, column).GetValue();
                const std::string& valB = this->GetCell(b, column).GetValue();
                return ascending ? valA < valB : valA > valB;
            }
        );
        
        // Create new sorted columns
        std::vector<std::vector<StringValue>> sortedColumns(m_columns.size());
        for (size_t col = 0; col < m_columns.size(); col++) {
            sortedColumns[col].resize(m_columns[col].size());
            for (size_t row = 0; row < indices.size(); row++) {
                sortedColumns[col][row] = m_columns[col][indices[row]];
            }
        }
        
        // Replace with sorted data
        m_columns = std::move(sortedColumns);
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
        MIR_ASSERT(false, "IMPLEMENT THIS");
        // if (values.size() != m_columns.size()) return;
        // if (rowIndex > RowCount()) rowIndex = RowCount();
        
        // for (size_t i = 0; i < m_columns.size(); i++) {
        //     m_columns[i].insert(m_columns[i].begin() + rowIndex, 
        //                     StringValue::CreateCopy(values[i]));
        // }
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

    void BaseTable::RenderContextMenuHeader()
    {
                // Default implementation
                if (ImGui::MenuItem("Clear Table")) {
                    Clear();
                }
                if (ImGui::MenuItem("Add Row")) {
                    std::vector<std::string> emptyRow(GetColumnCount(), "");
                    AddRow(emptyRow);
                }
                if (ImGui::MenuItem("Copy All")) {
                    std::string buffer;
                    size_t rowCount = RowCount();
                    size_t colCount = m_columnNames.size();
                    
                    // Create CSV format
                    for (size_t i = 0; i < rowCount; i++) {
                        for (size_t j = 0; j < colCount; j++) {
                            buffer += GetCell(i, j).GetValue();
                            if (j < colCount - 1) buffer += ",";
                        }
                        buffer += "\n";
                    }
                    ImGui::SetClipboardText(buffer.c_str());
                }
    }
    void BaseTable::RenderContextMenuColumns(int column)
    {
        if (column >= 0 && column < static_cast<int>(m_columnNames.size())) {
            if (ImGui::BeginMenu(("Column: " + m_columnNames[column]).c_str())) {
                if (ImGui::MenuItem("Sort Ascending")) {
                    SortByColumn(column, true);
                }
                if (ImGui::MenuItem("Sort Descending")) {
                    SortByColumn(column, false);
                }
                if (ImGui::MenuItem("Reset Width")) {
                    m_columnWidths[column] = 0.0f;
                }
                
                // Add more column-specific options
                ImGui::Separator();
                
                // Example: Change column title
                std::string columnName = m_columnNames[column];
                if (ImGui::InputText("Column Name", &columnName, 
                                     ImGuiInputTextFlags_EnterReturnsTrue)) {
                    m_columnNames[column] = columnName;
                }
                
                // Example: Show column specific data
                ImGui::Text("Column index: %d", column);
                ImGui::Text("Items: %zu", RowCount());
                
                ImGui::EndMenu();
            }
            
            ImGui::Separator();
            
            // General column operations
            if (ImGui::MenuItem("Hide Column")) {
                // Implement column hiding functionality
                // You'll need to add a vector<bool> m_columnVisible member
            }
            
            if (ImGui::MenuItem("Add Column")) {
                // Implement adding new columns
                // This will require restructuring the table
            }
            
            if (ImGui::MenuItem("Remove Column") && m_columnNames.size() > 1) {
                // Implement column removal
                // This will require restructuring the table
            }
        }
    }
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    // BASE TABLE END
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    void CsvTable::RenderContextMenuHeader() {
        if (ImGui::MenuItem("Generate typ"))
        {
            /* code */
        }
        
    }
    CsvTable::CsvTable() {}

    CsvTable::~CsvTable() {}

} // namespace UI
}  // namespace Mir