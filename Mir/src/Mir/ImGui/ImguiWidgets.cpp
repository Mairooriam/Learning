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