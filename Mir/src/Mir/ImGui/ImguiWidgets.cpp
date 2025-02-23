#include "ImguiWidgets.h"

namespace MirUI {
    //////////////////////////////////////////////////////
    ////////////// HELPER FUNCTIONS //////////////////////
    //////////////////////////////////////////////////////
    void ToggleKeyboardNavigationEnabled(std::optional<bool> enabled){
        ImGuiIO& io = ImGui::GetIO();
        if (!enabled.has_value()) {
            // Toggle mode
            bool isEnabled = io.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard;
            enabled = !isEnabled;
        }
        
        if (enabled.value()) {
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        } else {
            io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
        }
    }

    static void HelpMarker(const char* desc){
        ImGui::TextDisabled("(?)");
        if (ImGui::BeginItemTooltip())
        {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    static void PushStyleCompact()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, (float)(int)(style.FramePadding.y * 0.60f));
        ImGui::PushStyleVarY(ImGuiStyleVar_ItemSpacing, (float)(int)(style.ItemSpacing.y * 0.60f));
    }

        
    static void PopStyleCompact()
    {
        ImGui::PopStyleVar(2);
    }

    //////////////////////////////////////////////////////
    ////////////// HELPER FUNCTIONS //////////////////////
    //////////////////////////////////////////////////////



    //////////////////////////////////////////////////////
    //////////////      Widgets     //////////////////////
    //////////////////////////////////////////////////////
        bool InputTextWithSuggestions(const char* label, char* buf, size_t buf_size, const std::vector<std::string_view>& suggestions) {
            bool value_changed = ImGui::InputText(label, buf, buf_size);
            static int selected_index = -1;
    
            if (ImGui::IsItemActive()) {
                ToggleKeyboardNavigationEnabled(false);
                ImGui::BeginTooltip();
                for (int i = 0; i < suggestions.size(); ++i) {
                    const auto& suggestion = suggestions[i];
                    if (std::string_view(buf).empty() || suggestion.find(buf) != std::string_view::npos) {
                        bool is_selected = (i == selected_index);
                        if (ImGui::Selectable(suggestion.data(), is_selected)) {
                            strncpy(buf, suggestion.data(), buf_size);
                            buf[buf_size - 1] = '\0'; // Ensure null-termination
                            value_changed = true;
                            selected_index = -1; // Reset selection after choosing
                        }
                        if (is_selected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                }
                ImGui::EndTooltip();
    
                // Handle keyboard navigation
                if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
                    selected_index = (selected_index + 1) % suggestions.size();
                } else if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
                    selected_index = (selected_index - 1 + suggestions.size()) % suggestions.size();
                }
            } else {
                selected_index = -1; // Reset selection when input is not active
                ToggleKeyboardNavigationEnabled(true);
            }
    
            return value_changed;
        }
        void tableFromBrData(std::map<std::string, std::vector<Mir::brDataTypeNode>>& brData){
            // Expose a few Borders related flags interactively
            enum ContentsType { CT_Text, CT_FillButton };
            static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
            static bool display_headers = false;
            static int contents_type = CT_Text;

            PushStyleCompact();
            ImGui::CheckboxFlags("ImGuiTableFlags_RowBg", &flags, ImGuiTableFlags_RowBg);
            ImGui::CheckboxFlags("ImGuiTableFlags_Borders", &flags, ImGuiTableFlags_Borders);
            ImGui::SameLine(); HelpMarker("ImGuiTableFlags_Borders\n = ImGuiTableFlags_BordersInnerV\n | ImGuiTableFlags_BordersOuterV\n | ImGuiTableFlags_BordersInnerH\n | ImGuiTableFlags_BordersOuterH");
            ImGui::Indent();

            ImGui::CheckboxFlags("ImGuiTableFlags_BordersH", &flags, ImGuiTableFlags_BordersH);
            ImGui::Indent();
            ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuterH", &flags, ImGuiTableFlags_BordersOuterH);
            ImGui::CheckboxFlags("ImGuiTableFlags_BordersInnerH", &flags, ImGuiTableFlags_BordersInnerH);
            ImGui::Unindent();

            ImGui::CheckboxFlags("ImGuiTableFlags_BordersV", &flags, ImGuiTableFlags_BordersV);
            ImGui::Indent();
            ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuterV", &flags, ImGuiTableFlags_BordersOuterV);
            ImGui::CheckboxFlags("ImGuiTableFlags_BordersInnerV", &flags, ImGuiTableFlags_BordersInnerV);
            ImGui::Unindent();

            ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuter", &flags, ImGuiTableFlags_BordersOuter);
            ImGui::CheckboxFlags("ImGuiTableFlags_BordersInner", &flags, ImGuiTableFlags_BordersInner);
            ImGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
            ImGui::CheckboxFlags("ImGuiTableFlags_ContextMenuInBody", &flags, ImGuiTableFlags_ContextMenuInBody);
            ImGui::Unindent();

            ImGui::AlignTextToFramePadding(); ImGui::Text("Cell contents:");
            ImGui::SameLine(); ImGui::RadioButton("Text", &contents_type, CT_Text);
            ImGui::SameLine(); ImGui::RadioButton("FillButton", &contents_type, CT_FillButton);
            ImGui::Checkbox("Display headers", &display_headers);
            ImGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBody", &flags, ImGuiTableFlags_NoBordersInBody); ImGui::SameLine(); HelpMarker("Disable vertical borders in columns Body (borders will always appear in Headers");
            PopStyleCompact();    

            
            
            int COLUMNS_COUNT = 3;
      
            // Create persistent buffers if they don't exist
            static std::vector<std::vector<NodeBuffer>> buffers;

            // Resize buffers if data size changed
            if (buffers.size() != brData.size()) {
                buffers.resize(brData.size());
                size_t i = 0;
                for (const auto& [key, nodeData] : brData) {
                    buffers[i].resize(nodeData.size());
                    i++;
                }
            }

            size_t nodeDataIndex = 0;
            for (const auto& [key, nodeData] : brData) {
                // Create a bullet menu item for each key

                if (ImGui::CollapsingHeader(key.c_str())) {

                // If this key's content should be shown
                if (ImGui::BeginTable(("table_" + key).c_str(), COLUMNS_COUNT, flags)) {
                    if (display_headers) {
                        ImGui::TableSetupColumn("Name");
                        ImGui::TableSetupColumn("Type");
                        ImGui::TableSetupColumn("Comment");
                        ImGui::TableHeadersRow();
                    }

                    size_t row = 0;
                    for (const auto& data : nodeData) {
                        if (buffers[nodeDataIndex][row].name[0] == '\0') {
                            strncpy(buffers[nodeDataIndex][row].name.data(), data.name.c_str(), buffers[nodeDataIndex][row].name.size() - 1);
                            strncpy(buffers[nodeDataIndex][row].type.data(), data.type.c_str(), buffers[nodeDataIndex][row].type.size() - 1);
                            strncpy(buffers[nodeDataIndex][row].comment.data(), data.comment.c_str(), buffers[nodeDataIndex][row].comment.size() - 1);
                        }

                        ImGui::TableNextRow();
                        
                        ImGui::TableSetColumnIndex(0);
                        ImGui::PushItemWidth(-1);
                        if (ImGui::InputText(("##DataName" + key + std::to_string(row)).c_str(), 
                                            buffers[nodeDataIndex][row].name.data(), 
                                            buffers[nodeDataIndex][row].name.size())) {
                            // Handle input change
                        }
                        ImGui::PopItemWidth();

                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextUnformatted(data.type.c_str());
                        ImGui::SameLine();
                        MirUI::contextPopup(row, 1);

                        ImGui::TableSetColumnIndex(2);
                        ImGui::TextUnformatted(data.comment.c_str());

                        row++;
                    }
                    ImGui::EndTable();
                    }
                }
                nodeDataIndex++;
            }
        
        
        }
        void contextPopup(size_t row, size_t column)
        {
            ImGui::PushID((int)(row * 1000 + column)); // this need better implementation
            ImGui::SmallButton("..");
            bool popup_open = false;    
            if (ImGui::BeginPopupContextItem("##popup", ImGuiPopupFlags_MouseButtonLeft))
            {
                ImGui::Text("This is the popup for Button(\"..\") in Cell %d,%d", row, column);
                if (ImGui::Button("Close") || ImGui::IsKeyPressed(ImGuiKey_Enter))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
            ImGui::PopID();
        }
        //////////////////////////////////////////////////////
        //////////////      Widgets     //////////////////////
        //////////////////////////////////////////////////////
}