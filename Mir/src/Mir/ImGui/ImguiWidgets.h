#pragma once
#include "Mirpch.h"
#include <optional>
#include "WindowsFileBrowser.h"

// TODOS FOR IMGUI

// MULTI SELECT TABLE COLUMN WITH SHIFT

#include "imgui.h"
#include "b&rParser/brTypes.h"

class WindowsFileBrowser;  // Forward declaration

namespace MirUI {
    struct NodeBuffer {
        std::array<char, 256> name;    // Smaller buffer size for efficiency
        std::array<char, 128> type;    // Adjust sizes based on your needs
        std::array<char, 512> comment;
    };



    void ToggleKeyboardNavigationEnabled(std::optional<bool> enabled = std::nullopt);
    bool InputTextWithSuggestions(const char* label, char* buf, size_t buf_size, const std::vector<std::string_view>& suggestions);

    void tableFromBrData();
    void contextPopup(size_t row, size_t column);


    void treeNodeForTypes(const Mir::brTyp& typ);

    

    

}

