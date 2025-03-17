#include "varConfigHelper.h"

namespace Mir {
    void RenderIODataTable(const brVarConfigCollection& VarConfigCollection, bool displayCellSize) {
        size_t i = 0;
        for (auto& varConfig : VarConfigCollection) {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.0f, 1.0f));
            std::string label = "IO Data \t\t" + varConfig.comment + "###IO Data Table" + std::to_string(i);
            bool innerTreeOpen = ImGui::CollapsingHeader(label.c_str());
            ImGui::PopStyleVar();
    
            if (innerTreeOpen) {
                RenderVariableTable(varConfig, i, displayCellSize);
            }
            i++;
        }
    }
    
    void RenderVariableTable(const brVarConfigNode& varConfig, size_t tableIndex, bool displayCellSize) {
        static const float COLUMN_WIDTHS[] = {
            400.0f, // Name width
            78.0f,  // Type width
            60.0f,  // Value width
            60.0f   // Comment width
        };
        
        static const char* COLUMN_NAMES[] = {
            "IO Adress", 
            "Process Variable", 
            "Type", 
            "Comment"
        };
        
        if (displayCellSize) {
            float availWidth = ImGui::GetContentRegionAvail().x;
            ImGui::Text("Available width: %.1f", availWidth);
        }
    
        ImGui::Indent();
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0.0f, 2.0f));
        
        if (ImGui::BeginTable("Node", 4, tableFlags)) {
            // Setup columns
            for (int col = 0; col < 4; col++) {
                ImGui::TableSetupColumn(COLUMN_NAMES[col], ImGuiTableColumnFlags_WidthFixed, COLUMN_WIDTHS[col]);
            }
            ImGui::TableHeadersRow();
            ImGui::TableNextRow();
    
            // Display column sizes if needed
            if (displayCellSize) {
                for (int col = 0; col < 4; col++) {
                    ImGui::TableSetColumnIndex(col);
                    ImGui::Text("(w: %5.1f)", ImGui::GetColumnWidth(col));
                }
            }
            
            // Render each row
            RenderTableRows(varConfig, tableIndex);
            
            ImGui::EndTable();
        }
        
        ImGui::PopStyleVar();
        ImGui::Unindent();
    }
    
    void RenderTableRows(const brVarConfigNode& varConfig, size_t tableIndex) {
        static const char* FIELD_NAMES[] = {
            "ioAdress",
            "processVariable",
            "type",
            "comment"
        };
        
        size_t rowIndex = 0;
        for (auto& values : varConfig) {
            ImGui::TableNextRow();
            
            // Get references to the value fields
            const std::string* valueFields[] = {
                &values.ioAdress,
                &values.processVariable,
                &values.type,
                &values.comment
            };
            
            // Render each column in the row
            for (int col = 0; col < 4; col++) {
                ImGui::TableNextColumn();
                std::string label = "##" + std::string(FIELD_NAMES[col]) + 
                                   std::to_string(tableIndex) + "_" + std::to_string(rowIndex);
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::InputText(label.c_str(), valueFields[col]);
                ImGui::PopItemWidth();
            }
            
            rowIndex++;
        }
    }
    
    void RenderImGuiEditor(brparser& brparser) {
        ImGui::Begin(".iom editor");
        
        brVarConfigCollection VarConfigCollection = brparser.getVarConfig();
        
        static bool displayCellSize = false;
        if (ImGui::TreeNode("Table Flags")) {
            ImGui::Checkbox("display table cell size", &displayCellSize);
            ImGui::TreePop();
        }
    
        static bool treeOpen = false;
        if (!VarConfigCollection.empty()) {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.0f, 1.0f));
            treeOpen = ImGui::CollapsingHeader("IO Data Table###IO Data Table");
            ImGui::PopStyleVar();
            ImGui::Indent();
        }
        
        if (treeOpen) {
            RenderIODataTable(VarConfigCollection, displayCellSize);
            ImGui::Unindent();
        }
    
        ImGui::End();
    }
    
}
