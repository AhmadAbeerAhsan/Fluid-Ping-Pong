#include "UIWidgets.hpp"

namespace
{
    struct ScopedFontScale
    {
        explicit ScopedFontScale(float scale) { ImGui::SetWindowFontScale(scale); }
        ~ScopedFontScale() { ImGui::SetWindowFontScale(1.0f); }
    };
}

bool UIWidgets::Button(const char* label, const UIStyle& style, ImVec2 size, HorizontalLayout hl)
{
    ScopedFontScale fontScale(style.fontScale);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, style.padding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleColor(ImGuiCol_Button,        style.color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.hoverColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  style.activeColor);
    ImGui::PushStyleColor(ImGuiCol_Text,          style.textColor);

    float effectiveWidth = size.x;
    if (effectiveWidth <= 0.0f)
    {
        ImVec2 textSize = ImGui::CalcTextSize(label);
        effectiveWidth = textSize.x + style.padding.x * 2.0f;
    }
    switch (hl)
    {
    case HorizontalLayout::Left:
        ImGui::SetCursorPosX(0.0f);
        break;
    case HorizontalLayout::Middle:
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - effectiveWidth) * 0.5f);
        break;
    case HorizontalLayout::Right:
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - effectiveWidth);
        break;
    default:
        break;
    }
    
    bool pressed = ImGui::Button(label, size);

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(2);
    return pressed;
}

void UIWidgets::Label(const char* text, const UIStyle& style, HorizontalLayout hl)
{
    ScopedFontScale fontScale(style.fontScale);

    ImVec2 textSize = ImGui::CalcTextSize(text);   // already reflects style.fontScale -- no further scaling needed
    float cursorPosX{0.0f};

    switch (hl)
    {
    case HorizontalLayout::Left:
        cursorPosX = style.padding.x;
        break;
    case HorizontalLayout::Middle:
        cursorPosX = (ImGui::GetWindowWidth() - textSize.x) * 0.5f;
        break;
    case HorizontalLayout::Right:
        cursorPosX = ImGui::GetWindowWidth() - textSize.x - style.padding.x;
        break;
    default:
        break;
    }

    ImGui::SetCursorPosX(cursorPosX);
    ImGui::Dummy(ImVec2(0.0f, style.padding.y));
    ImGui::SetCursorPosX(cursorPosX);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);
    ImGui::TextUnformatted(text);
    ImGui::PopStyleColor();
    ImGui::SetCursorPosX(cursorPosX);
    ImGui::Dummy(ImVec2(0.0f, style.padding.y));
}

bool UIWidgets::Slider(const char* label, float* value, float min, float max, const UIStyle& style, float width, HorizontalLayout hl)
{
    ScopedFontScale fontScale(style.fontScale);
    ImVec2 textSize = ImGui::CalcTextSize(label);

    if (width > 0.0f)
        ImGui::SetNextItemWidth(width);

    float effectiveWidth = width + textSize.x + style.padding.x;
    
    float cursorPosX{0.0f};

    switch (hl)
    {
    case HorizontalLayout::Left:
        cursorPosX = style.padding.x;
        break;
    case HorizontalLayout::Middle:
        cursorPosX = (ImGui::GetWindowWidth() - textSize.x) * 0.5f;
        break;
    case HorizontalLayout::Right:
        cursorPosX = ImGui::GetWindowWidth() - textSize.x - style.padding.x;
        break;
    default:
        break;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, style.padding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleColor(ImGuiCol_FrameBg,          style.darkS);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,   style.darkS);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive,    style.darkP);
    ImGui::PushStyleColor(ImGuiCol_SliderGrab,       style.darkP);
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, style.lightP);
    ImGui::PushStyleColor(ImGuiCol_Text,             style.textColor);

    ImGui::SetCursorPosX(cursorPosX);
    bool changed = ImGui::SliderFloat(label, value, min, max);

    ImGui::PopStyleColor(6);
    ImGui::PopStyleVar(2);
    return changed;
}

bool UIWidgets::InputField(const char* label, char* buffer, size_t bufferSize, const UIStyle& style, HorizontalLayout hl)
{
    ScopedFontScale fontScale(style.fontScale);

    ImVec2 textSize = ImGui::CalcTextSize(label);
    textSize.x *= style.fontScale;

    float boxWidth = ImGui::CalcItemWidth()/3.0f;
    float spacing  = ImGui::GetStyle().ItemInnerSpacing.x;
    float effectiveWidth = textSize.x + spacing + boxWidth;

    float cursorPosX{0.0f};
    switch (hl)
    {
    case HorizontalLayout::Left:
        cursorPosX = style.padding.x;
        break;
    case HorizontalLayout::Middle:
        cursorPosX = (ImGui::GetWindowWidth() - effectiveWidth) * 0.5f;
        break;
    case HorizontalLayout::Right:
        cursorPosX = ImGui::GetWindowWidth() - effectiveWidth - style.padding.x;
        break;
    default:
        break;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, style.padding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleColor(ImGuiCol_FrameBg,        style.color);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, style.hoverColor);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  style.activeColor);
    ImGui::PushStyleColor(ImGuiCol_Text,           style.textColor);

    ImGui::SetCursorPosX(cursorPosX);
    ImGui::TextUnformatted(label);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(boxWidth);
    std::string hiddenLabel = std::string("##") + label; // keeps a stable ImGui ID without drawing a second visible label
    bool changed = ImGui::InputText(hiddenLabel.c_str(), buffer, bufferSize);

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(2);
    return changed;
}

bool UIWidgets::Checkbox(
    const char* label,
    bool* value,
    const UIStyle& style,
    HorizontalLayout hl)
{
    ScopedFontScale fontScale(style.fontScale);

    ImVec2 textSize = ImGui::CalcTextSize(label);

    float cursorPosX{0.0f};

    switch (hl)
    {
    case HorizontalLayout::Left:
        cursorPosX = style.padding.x;
        break;

    case HorizontalLayout::Middle:
        cursorPosX = (ImGui::GetWindowWidth() - textSize.x) * 0.5f;
        break;

    case HorizontalLayout::Right:
        cursorPosX = ImGui::GetWindowWidth() - textSize.x - style.padding.x;
        break;

    default:
        break;
    }

    ImGui::PushStyleVar(
        ImGuiStyleVar_FramePadding,
        style.padding
    );

    ImGui::PushStyleVar(
        ImGuiStyleVar_FrameRounding,
        style.rounding
    );

    // Checkbox colors
    ImGui::PushStyleColor(
        ImGuiCol_FrameBg,
        style.darkS
    );

    ImGui::PushStyleColor(
        ImGuiCol_FrameBgHovered,
        style.darkS
    );

    ImGui::PushStyleColor(
        ImGuiCol_FrameBgActive,
        style.darkP
    );

    ImGui::PushStyleColor(
        ImGuiCol_CheckMark,
        style.lightP
    );

    ImGui::PushStyleColor(
        ImGuiCol_Text,
        style.textColor
    );

    ImGui::SetCursorPosX(cursorPosX);

    bool changed = ImGui::Checkbox(
        label,
        value
    );

    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar(2);

    return changed;
}