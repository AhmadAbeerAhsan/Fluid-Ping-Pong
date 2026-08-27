#include "UIWidgets.hpp"

namespace
{
    struct ScopedFontScale
    {
        explicit ScopedFontScale(float scale) { ImGui::SetWindowFontScale(scale); }
        ~ScopedFontScale() { ImGui::SetWindowFontScale(1.0f); }
    };
}

bool UIWidgets::Button(const char* label, float font_size, HorizontalLayout hl)
{
    ScopedFontScale fontScale(font_size);
    ImVec2 size = ImGui::CalcTextSize(label);
    size.y += 10 + style.padding.y;
    size.x += 2.0f * style.padding.x;

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

void UIWidgets::Label(const char* text, float font_size, HorizontalLayout hl)
{
    ScopedFontScale fontScale(font_size);
    ImVec2 textSize = ImGui::CalcTextSize(text);   // already reflects style.fontScale -- no further scaling needed
    textSize.y += 10 + style.padding.y;
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

bool UIWidgets::Slider(const char* label, float* value, float min, float max, float font_size, float width, HorizontalLayout hl)
{
    ScopedFontScale fontScale(font_size);
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

bool UIWidgets::InputField(const char* label, char* buffer, size_t bufferSize, float font_size, HorizontalLayout hl)
{
    ScopedFontScale fontScale(font_size);

    ImVec2 textSize = ImGui::CalcTextSize(label);
    textSize.x *= font_size;

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
    bool* value, float font_size,
    HorizontalLayout hl)
{
    ScopedFontScale fontScale(font_size);

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

void UIWidgets::OnlineMatch(const char *match_name, const char *player_count, std::function<void()> fun, float font_size)
{
    float display_width = ImGui::GetWindowWidth();
    ScopedFontScale fontScale(font_size);
    ImVec2 match_name_size = ImGui::CalcTextSize(match_name);
    match_name_size.y += 10 + style.padding.y;

    ImGui::Text("%s", match_name);

    std::string player_count_ =  std::string(player_count) + "/2";
    ImVec2 player_count_size = ImGui::CalcTextSize(player_count);
    player_count_size.y += 10 + style.padding.y;

    ImGui::SameLine((display_width - player_count_size.x)/2.0f);
    ImGui::Text("%s", player_count_.c_str());

    std::string join{"Join"};
    ImVec2 join_size = ImGui::CalcTextSize(join.c_str());
    join_size.y += 10 + style.padding.y;
    join_size.x += 2.0f * style.padding.x;
    ImGui::SameLine(display_width - join_size.x);
    if (ImGui::Button(join.c_str(), join_size))
    {
        fun();
    }
    
}
