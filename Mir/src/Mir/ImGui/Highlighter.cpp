#include "Highlighter.h"
#include "imgui.h"

namespace Mir {
    Highlighter::Tab(){
        ImGui::PushStyleColor(ImGuiCol_TabActive, IM_COL32(0xff, 0x00, 0xff, 0xff));
    }
}
