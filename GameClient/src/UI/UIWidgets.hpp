#pragma once

#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <cstddef>
#include <iostream>

struct UIStyle
{
    ImVec4 color       = ImVec4(0.10f, 0.20f, 0.45f, 1.0f); // base fill (button / track / grab)
    ImVec4 hoverColor  = ImVec4(0.30f, 0.55f, 0.95f, 1.0f);
    ImVec4 activeColor = ImVec4(0.10f, 0.25f, 0.40f, 1.0f);
    ImVec4 textColor   = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    float  fontScale   = 1.0f;
    ImVec2 padding     = ImVec2(8.0f, 6.0f);
    float  rounding    = 6.0f;

    ImVec4 lightP = ImVec4(0.44f, 0.45f, 0.9f, 1.0f);
    ImVec4 lightS   = ImVec4(0.14f, 0.15f, 0.8f, 1.0f);
    ImVec4 darkP = ImVec4(0.2f, 0.2f, 0.4f, 1.0f);
    ImVec4 darkS   = ImVec4(0.1f, 0.1f, 0.3f, 1.0f);
};

class UIWidgets
{
public:
    enum HorizontalLayout{
        Left,
        Middle,
        Right
    };

    static bool Button(const char* label, const UIStyle& style, ImVec2 size = ImVec2(0.0f, 0.0f), HorizontalLayout hl = HorizontalLayout::Left);
    static void Label(const char* text, const UIStyle& style, HorizontalLayout hl = HorizontalLayout::Left);
    static bool Slider(const char* label, float* value, float min, float max, const UIStyle& style, float width = 0.0f, HorizontalLayout hl = HorizontalLayout::Left);
    static bool InputField(const char* label, char* buffer, size_t bufferSize, const UIStyle& style, HorizontalLayout hl = HorizontalLayout::Left);
    static bool Checkbox(const char* label, bool* value, const UIStyle& style, HorizontalLayout hl = HorizontalLayout::Left);
};
