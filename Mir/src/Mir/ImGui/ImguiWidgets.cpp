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
        void multilineTextClipboard()
        {
            bool clipboard_valid = false;
            const char* clipboard_text = nullptr;
                
            try {
                // Capture potential ImGui errors/warnings
                clipboard_text = ImGui::GetClipboardText();
                clipboard_valid = (clipboard_text && clipboard_text[0] != '\0');
            } catch (...) {
                clipboard_valid = false;
            }
            
            if (clipboard_valid) {
                static bool show_special_chars = false;
                ImGui::Checkbox("Show special characters", &show_special_chars);
                ImGui::SameLine();
                // Add height slider (default range 3-20 lines)
                static float textHeightMultiplier = 3.0f;
                ImGui::PushItemWidth(200.0f); 
                ImGui::SliderFloat("Text area height", &textHeightMultiplier, 3.0f, 100.0f, "%.0f lines");
                ImGui::PopItemWidth();
                // Create a child window with scrolling for large clipboard content
                ImGui::BeginChild("Clipboard", ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * textHeightMultiplier), true);
                
                if (show_special_chars) {
                    // Display text with visible special characters
                    for (const char* c = clipboard_text; *c != '\0'; c++) {
                        if (*c == '\n') ImGui::TextUnformatted("\\n");
                        else if (*c == '\r') ImGui::TextUnformatted("\\r");
                        else if (*c == '\t') ImGui::TextUnformatted("\\t");
                        else if (*c < 32) ImGui::Text("\\x%02X", (unsigned char)*c); // Other control chars
                        else ImGui::Text("%c", *c);
                        ImGui::SameLine(0.0f, 0.0f);
                    }
                } else {
                    // Original display mode
                    ImGui::TextWrapped("%s", clipboard_text);
                }
                
                ImGui::EndChild();
            } else {
                ImGui::TextDisabled("No text content in clipboard");
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




        void treeNodeForTypes(const Mir::brTyp& typ){

        }


        void RenderTableRows(Mir::brVarConfigNode& varConfig, size_t tableIndex) {
            static const char* FIELD_NAMES[] = {
                "ioAdress",
                "processVariable",
                "type",
                "comment"
            };
            
            size_t rowIndex = 0;
            for (auto& values : varConfig) {
                ImGui::TableNextRow();
                
                // Get references to the value fields - no const_cast needed
                std::string* valueFields[] = {
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
                    if (ImGui::InputText(label.c_str(), valueFields[col])) {
                        // Handle changes here if needed
                    }
                    ImGui::PopItemWidth();
                }
                
                rowIndex++;
            }
        }
        //////////////////////////////////////////////////////
        //////////////      Widgets     //////////////////////
        //////////////////////////////////////////////////////
}