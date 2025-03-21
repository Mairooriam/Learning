#pragma once
#include "imgui.h"
#include <vector>
#include <string>
#include <functional>

namespace Mir {
    // Style applying function type
    using StyleApplyFunc = void(*)();
    
    // Style definition structure
    struct Style {
        const char* name;
        StyleApplyFunc apply;
    };

    // Forward declarations of style functions
    void ApplyMirDarkStyle();
    void ApplyMirLightStyle();
    void ApplyMedakkPurpleStyle();
    
    // Array of available styles
    inline Style g_Styles[] = {
        { "Mir Dark", ApplyMirDarkStyle },
        { "Mir Light", ApplyMirLightStyle },
        { "Medakk Purple", ApplyMedakkPurpleStyle }
    };
    
    // Function to apply a style by index
    inline bool SetStyle(int styleIndex) {
        const int numStyles = sizeof(g_Styles) / sizeof(g_Styles[0]);
        if (styleIndex >= 0 && styleIndex < numStyles) {
            g_Styles[styleIndex].apply();
            return true;
        }
        return false;
    }
    
    // Implementation of the style functions
    inline void ApplyMirDarkStyle() {
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.48f, 0.32f, 0.16f, 0.54f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.98f, 0.65f, 0.26f, 0.40f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.98f, 0.65f, 0.26f, 0.67f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.48f, 0.33f, 0.16f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.98f, 0.65f, 0.26f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.88f, 0.59f, 0.24f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(1.00f, 0.66f, 0.27f, 1.00f);
        colors[ImGuiCol_Button]                 = ImVec4(1.00f, 0.66f, 0.27f, 0.40f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.98f, 0.65f, 0.26f, 1.00f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.98f, 0.56f, 0.06f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.98f, 0.65f, 0.26f, 0.31f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(1.00f, 0.54f, 0.00f, 0.91f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.98f, 0.65f, 0.26f, 1.00f);
        colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.75f, 0.45f, 0.10f, 0.78f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.75f, 0.45f, 0.10f, 1.00f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.98f, 0.65f, 0.26f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.98f, 0.65f, 0.26f, 0.67f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.98f, 0.65f, 0.26f, 0.95f);
        colors[ImGuiCol_InputTextCursor]        = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TabHovered]             = ImVec4(0.98f, 0.65f, 0.26f, 0.80f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.58f, 0.40f, 0.18f, 0.86f);
        colors[ImGuiCol_TabSelected]            = ImVec4(0.68f, 0.46f, 0.20f, 1.00f);
        colors[ImGuiCol_TabSelectedOverline]    = ImVec4(0.98f, 0.65f, 0.26f, 1.00f);
        colors[ImGuiCol_TabDimmed]              = ImVec4(0.15f, 0.11f, 0.07f, 0.97f);
        colors[ImGuiCol_TabDimmedSelected]      = ImVec4(0.42f, 0.29f, 0.14f, 1.00f);
        colors[ImGuiCol_TabDimmedSelectedOverline]  = ImVec4(0.50f, 0.50f, 0.50f, 0.00f);
        colors[ImGuiCol_DockingPreview]         = ImVec4(0.98f, 0.65f, 0.26f, 0.70f);
        colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextLink]               = ImVec4(0.98f, 0.67f, 0.26f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.98f, 0.65f, 0.26f, 0.35f);
        colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavCursor]              = ImVec4(0.98f, 0.65f, 0.26f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }
    
    inline void ApplyMirLightStyle() {
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text]                   = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]                = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
        colors[ImGuiCol_Border]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg]                = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.98f, 0.65f, 0.26f, 0.40f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.98f, 0.65f, 0.26f, 0.67f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.98f, 0.65f, 0.26f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.98f, 0.65f, 0.26f, 0.78f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.80f, 0.64f, 0.46f, 0.60f);
        colors[ImGuiCol_Button]                 = ImVec4(0.98f, 0.65f, 0.26f, 0.40f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.98f, 0.65f, 0.26f, 1.00f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.98f, 0.56f, 0.06f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.98f, 0.65f, 0.26f, 0.31f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.98f, 0.65f, 0.26f, 0.80f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.98f, 0.65f, 0.26f, 1.00f);
        colors[ImGuiCol_Separator]              = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.80f, 0.50f, 0.14f, 0.78f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.80f, 0.50f, 0.14f, 1.00f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.98f, 0.65f, 0.26f, 0.67f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.98f, 0.65f, 0.26f, 0.95f);
        colors[ImGuiCol_InputTextCursor]        = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TabHovered]             = ImVec4(0.98f, 0.65f, 0.26f, 0.80f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.76f, 0.80f, 0.84f, 0.93f);
        colors[ImGuiCol_TabSelected]            = ImVec4(0.88f, 0.75f, 0.60f, 1.00f);
        colors[ImGuiCol_TabSelectedOverline]    = ImVec4(0.98f, 0.65f, 0.26f, 1.00f);
        colors[ImGuiCol_TabDimmed]              = ImVec4(0.92f, 0.93f, 0.94f, 0.99f);
        colors[ImGuiCol_TabDimmedSelected]      = ImVec4(0.91f, 0.83f, 0.74f, 1.00f);
        colors[ImGuiCol_TabDimmedSelectedOverline]  = ImVec4(0.26f, 0.59f, 1.00f, 0.00f);
        colors[ImGuiCol_DockingPreview]         = ImVec4(0.98f, 0.65f, 0.26f, 0.22f);
        colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines]              = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.98f, 0.89f, 0.78f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.64f, 0.61f, 0.57f, 1.00f);
        colors[ImGuiCol_TableBorderLight]       = ImVec4(0.74f, 0.71f, 0.68f, 1.00f);
        colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]          = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
        colors[ImGuiCol_TextLink]               = ImVec4(0.98f, 0.65f, 0.26f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.98f, 0.65f, 0.26f, 0.35f);
        colors[ImGuiCol_DragDropTarget]         = ImVec4(0.98f, 0.65f, 0.26f, 0.95f);
        colors[ImGuiCol_NavCursor]              = ImVec4(0.98f, 0.65f, 0.26f, 0.80f);
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
        
    }
    
    inline void ApplyMedakkPurpleStyle() {
            bool bStyleDark_ = true;
            float alpha_ = 0.5f;
            ImGuiStyle& style = ImGui::GetStyle();
            
            // light style from Pacôme Danhiez (user itamago) https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
            style.Alpha = 1.0f;
            style.FrameRounding = 3.0f;
            style.Colors[ImGuiCol_Text]                  = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
            style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
            //style.Colors[ImGuiCol_ChildWindowBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            style.Colors[ImGuiCol_PopupBg]               = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
            style.Colors[ImGuiCol_Border]                = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
            style.Colors[ImGuiCol_BorderShadow]          = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
            style.Colors[ImGuiCol_FrameBg]               = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
            style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
            style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
            style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
            style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
            style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
            style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
            //style.Colors[ImGuiCol_ComboBg]               = ImVec4(0.86f, 0.86f, 0.86f, 0.99f);
            style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
            style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            style.Colors[ImGuiCol_Button]                = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
            style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
            style.Colors[ImGuiCol_Header]                = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
            style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
            style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            //style.Colors[ImGuiCol_Column]                = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
            //style.Colors[ImGuiCol_ColumnHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
            //style.Colors[ImGuiCol_ColumnActive]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
            style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            //style.Colors[ImGuiCol_CloseButton]           = ImVec4(0.59f, 0.59f, 0.59f, 0.50f);
            //style.Colors[ImGuiCol_CloseButtonHovered]    = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
            //style.Colors[ImGuiCol_CloseButtonActive]     = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
            style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
            style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
            style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
            style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
            //style.Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
    
            if( bStyleDark_ )
            {
                for (int i = 0; i <= ImGuiCol_COUNT; i++)
                {
                    ImVec4& col = style.Colors[i];
                    float H, S, V;
                    ImGui::ColorConvertRGBtoHSV( col.x, col.y, col.z, H, S, V );
    
                    if( S < 0.1f )
                    {
                       V = 1.0f - V;
                    }
                    ImGui::ColorConvertHSVtoRGB( H, S, V, col.x, col.y, col.z );
                    if( col.w < 1.00f )
                    {
                        col.w *= alpha_;
                    }
                }
            }
            else
            {
                for (int i = 0; i <= ImGuiCol_COUNT; i++)
                {
                    ImVec4& col = style.Colors[i];
                    if( col.w < 1.00f )
                    {
                        col.x *= alpha_;
                        col.y *= alpha_;
                        col.z *= alpha_;
                        col.w *= alpha_;
                    }
                }
            }
        }
    // Helper function to get style count
    inline int GetStyleCount() {
        return sizeof(g_Styles) / sizeof(g_Styles[0]);
    }
    
    // Helper function to get style name by index
    inline const char* GetStyleName(int index) {
        const int numStyles = GetStyleCount();
        if (index >= 0 && index < numStyles)
            return g_Styles[index].name;
        return "Unknown";
    }
}

