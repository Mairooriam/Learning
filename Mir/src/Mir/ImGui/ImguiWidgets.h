#pragma once
#include "Mirpch.h"
#include <optional>

// TODOS FOR IMGUI

// MULTI SELECT TABLE COLUMN WITH SHIFT

#include "imgui.h"

namespace MirUI {

    void ToggleKeyboardNavigationEnabled(std::optional<bool> enabled = std::nullopt);
    bool InputTextWithSuggestions(const char* label, char* buf, size_t buf_size, const std::vector<std::string_view>& suggestions);
    void contextPopup(size_t row, size_t column);
}

