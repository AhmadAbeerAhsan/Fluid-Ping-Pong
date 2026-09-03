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

void UIWidgets::OnlineMatch(GameSessionData& session, std::function<void(int)> fun, float font_size)
{
    float display_width = ImGui::GetWindowWidth();
    ScopedFontScale fontScale(font_size);

    const char* match_name = session.MatchName();
    ImGui::Text("%s", match_name);

    std::string player_count_str = std::to_string(session.PlayerCount()) + "/2";
    ImVec2 player_count_size = ImGui::CalcTextSize(player_count_str.c_str());
    player_count_size.y += 10 + style.padding.y;

    ImGui::SameLine((display_width - player_count_size.x) / 2.0f);
    ImGui::Text("%s", player_count_str.c_str());

    bool red_free   = session.red_connection_status   == GameSessionData::ConnectionStatus::Disconnected;
    bool green_free = session.green_connection_status == GameSessionData::ConnectionStatus::Disconnected;

    std::string red_label   = red_free   ? "Join Red"   : "Red Taken";
    std::string green_label = green_free ? "Join Green" : "Green Taken";

    ImVec2 red_size = ImGui::CalcTextSize(red_label.c_str());
    red_size.y += 10 + style.padding.y;
    red_size.x += 2.0f * style.padding.x;

    ImVec2 green_size = ImGui::CalcTextSize(green_label.c_str());
    green_size.y += 10 + style.padding.y;
    green_size.x += 2.0f * style.padding.x;

    float total_width = red_size.x + green_size.x + style.padding.x;
    ImGui::SameLine(display_width - total_width);

    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.70f, 0.10f, 0.10f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.85f, 0.20f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.60f, 0.05f, 0.05f, 1.0f));
    ImGui::BeginDisabled(!red_free);
    if (ImGui::Button(red_label.c_str(), red_size))
    {
        fun(static_cast<int>(GameEventData::ObjectType::Red));
    }
    ImGui::EndDisabled();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.10f, 0.60f, 0.10f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.20f, 0.75f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.05f, 0.50f, 0.05f, 1.0f));
    ImGui::BeginDisabled(!green_free);
    if (ImGui::Button(green_label.c_str(), green_size))
    {
        fun(static_cast<int>(GameEventData::ObjectType::Green));
    }
    ImGui::EndDisabled();
    ImGui::PopStyleColor(3);
}
