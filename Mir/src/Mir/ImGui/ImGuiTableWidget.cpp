#include "ImGuiTableWidget.h"
#include <imgui.h>
#include <vector>

namespace Mir {
namespace Widgets {
    
        ////////////////////////////////////////
        //////// DATA RENDERER /////////////////
        ////////////////////////////////////////
        template<typename T>
        void DataRenderer<T>::Render(const T& data) {
            // Generic fallback that works for both primitive types and string-like types
            if constexpr (std::is_arithmetic_v<T>) {
                // For numeric types
                ImGui::Text("Value: %d", static_cast<int>(data));
            }
            else if constexpr (std::is_same_v<T, std::string>) {
                // For std::string
                ImGui::Text("String: %s", data.c_str());
            }
            else {
                // For other types
                ImGui::Text("Unknown type");
            }
        }

        // Specialization for brTyp
        void DataRenderer<brTyp>::Render(const brTyp& data) {
            ImGui::Text("Type: %s", "ADD DATA NAME HERE");
            //std::string label = "Collection" + std::to_string(i) + "\t\t"  + data.collections[i].comment + "###Collection" + std::to_string(i) + data.collections[i].comment;
            //bool treeOpen = ImGui::CollapsingHeader(label.c_str());
            if (ImGui::TreeNode("Details")) {
                // Render additional details specific to brTyp
                ImGui::Text("Collections: %d", data.size());
                // etc.
                ImGui::TreePop();
            }
        }

        // Specialization for varConfig
        void DataRenderer<brVarConfigCollection>::Render(const brVarConfigCollection& data) {
            ImGui::Text("VarConfig: %s", "ADD VAR CONFIG DATA HERE");
            // etc.
        }

       
        template class DataRenderer<int>; // TODO: REMOVE THIS LATER
        ////////////////////////////////////////
        //////// DATA RENDERER /////////////////
        ////////////////////////////////////////



        ////////////////////////////////////////
        //////// TABLE WIDGET //////////////////
        ////////////////////////////////////////
        template<typename T>
        void TableWidget<T>::Render() {
            if (m_DataPtr == nullptr) {
                ImGui::Text("No data to render");
                return;
            }

            ApplyTableStyles();
            
            if (ImGui::BeginTable(m_Title.c_str(), m_ColumnCount, m_Settings.GetTableFlags())) {
                // Render table header
                RenderTableHeader();
                
                // Render data
                RenderTableData(*m_DataPtr);
                
                ImGui::EndTable();
            }
            
            PopTableStyles();
        }
        
        template<typename T>
        void TableWidget<T>::RenderTableHeader() {
            // Setup columns based on type - you may want to customize this for each type
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 150.0f);
            ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableSetupColumn("Details", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();
            
            // Optionally show header widths
            if (m_Settings.GetShowHeaderWidth()) {
                ImGui::TableNextRow();
                for (int col = 0; col < m_ColumnCount; col++) {
                    ImGui::TableSetColumnIndex(col);
                    ImGui::Text("(w: %.1f)", ImGui::GetColumnWidth(col));
                }
            }
        }

        
        template<typename T>
        void TableWidget<T>::RenderTableRow(const T& data) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            
            // Use the DataRenderer to render the data
            DataRenderer<T>::Render(data);
        }
        
        template<typename T>
        void TableWidget<T>::RenderTableData(const T& data, int depth) {
            // For simple types, just render a single row
            RenderTableRow(data);
        }
        
        // Explicit template instantiations - add all types you'll use
        template class TableWidget<brTyp>;
        template class TableWidget<brVarConfigCollection>;
        template class TableWidget<int>; 
        ////////////////////////////////////////
        //////// TABLE WIDGET //////////////////
        ////////////////////////////////////////

}
}