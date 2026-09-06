#include "UI.hpp"

UI::UI(GLFWwindow *window) :
    m_audio_player()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io_ptr = &ImGui::GetIO();
    m_io_ptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    m_audio_player.init();
    m_audio_player.playMusic();
}

void UI::SetupUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::RenderUI()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool UI::StyledButton(const char* label, ImVec2 size)
{
    // Placeholder palette -- tune to your actual art direction.
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.20f, 0.45f, 0.85f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.55f, 0.95f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.15f, 0.35f, 0.70f, 1.0f));

    bool pressed = ImGui::Button(label, size);

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();
    return pressed;
}

bool UI::CenteredButton(const char* label, float width, float height)
{
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - width) * 0.5f);
    return StyledButton(label, ImVec2(width, height));
}

void UI::CenteredText(const char* text, float fontScale)
{
    ImGui::SetWindowFontScale(fontScale);
    ImVec2 textSize = ImGui::CalcTextSize(text);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - textSize.x) * 0.5f);
    ImGui::TextUnformatted(text);
    ImGui::SetWindowFontScale(1.0f);
}

void UI::Label(const char* text, ImVec4 color)
{
    ImGui::PushStyleColor(ImGuiCol_Text, color);
    ImGui::TextUnformatted(text);
    ImGui::PopStyleColor();
}

void UI::DrawScoreHUD(const std::string& leftName, int leftScore,
                       const std::string& rightName, int rightScore,
                       int red_ping_ms, int green_ping_ms, bool is_online)
{
    float display_width = DisplaySizeX();

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(display_width, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.55f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 12.0f));
    ImGui::Begin(
        "ScoreHUD", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav
    );

    ImGui::SetWindowFontScale(2.0f);

    float topY = ImGui::GetCursorPosY(); // anchor point for the right-side button stack

    std::string score_text = std::to_string(leftScore) + " : " + std::to_string(rightScore);

    ImVec2 leftNameSize = ImGui::CalcTextSize(leftName.c_str());
    ImVec2 scoreSize    = ImGui::CalcTextSize(score_text.c_str());

    float leftNameX  = display_width * 0.38f - leftNameSize.x;
    float rightNameX = display_width * 0.62f;
    float scoreX     = display_width * 0.5f - scoreSize.x * 0.5f;

    // ---- Row 1: names + score ----
    ImGui::SetCursorPosX(leftNameX);
    ImGui::TextColored(ImVec4(1.0f, 0.35f, 0.35f, 1.0f), "%s", leftName.c_str());

    ImGui::SameLine(scoreX);
    ImGui::Text("%s", score_text.c_str());

    ImGui::SameLine(rightNameX);
    ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "%s", rightName.c_str());

    // ---- Row 2: ping, centered under each name (online matches only) ----
    if (is_online)
    {
        ImGui::SetWindowFontScale(1.2f);

        auto PingColor = [](int ping) -> ImVec4 {
            return ping < 60  ? ImVec4(0.4f, 1.0f, 0.4f, 1.0f)
                 : ping < 120 ? ImVec4(1.0f, 0.85f, 0.3f, 1.0f)
                              : ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
        };

        std::string red_ping_text   = std::to_string(red_ping_ms)   + " ms";
        std::string green_ping_text = std::to_string(green_ping_ms) + " ms";

        ImVec2 redPingSize   = ImGui::CalcTextSize(red_ping_text.c_str());
        ImVec2 greenPingSize = ImGui::CalcTextSize(green_ping_text.c_str());
        float greenNameSizeX = ImGui::CalcTextSize(rightName.c_str()).x;

        float redPingCenterX   = leftNameX + leftNameSize.x * 0.5f - redPingSize.x * 0.5f;
        float greenPingCenterX = rightNameX + greenNameSizeX * 0.5f - greenPingSize.x * 0.5f;

        ImGui::SetCursorPosX(redPingCenterX);
        ImGui::TextColored(PingColor(red_ping_ms), "%s", red_ping_text.c_str());

        ImGui::SameLine(greenPingCenterX);
        ImGui::TextColored(PingColor(green_ping_ms), "%s", green_ping_text.c_str());

        ImGui::SetWindowFontScale(2.0f);
    }

    // ---- Right side: Quit (top), Settings (below), stacked independently of the rows above ----
    std::string quit{"Quit"};
    ImVec2 quit_size = ImGui::CalcTextSize(quit.c_str());
    quit_size.x += 20;
    quit_size.y += 10;

    std::string settings{"Settings"};
    ImVec2 settings_size = ImGui::CalcTextSize(settings.c_str());
    settings_size.x += 20;
    settings_size.y += 10;

    float right_margin = 10.0f;
    float button_gap   = 8.0f;

    float quitX     = display_width - quit_size.x - right_margin;
    float settingsX = display_width - settings_size.x - right_margin;
    float quitY     = topY;
    float settingsY = topY + quit_size.y + button_gap;

    ImGui::SetCursorPos(ImVec2(quitX, quitY));
    if (ImGui::Button(quit.c_str(), quit_size))
    {
        m_home_requested = true;
    }

    ImGui::SetCursorPos(ImVec2(settingsX, settingsY));
    if (ImGui::Button(settings.c_str(), settings_size))
    {
        m_show_settings = !m_show_settings;
    }

    ImGui::SetWindowFontScale(1.0f);

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

std::string UI::CreateDirectionString(bool is_red)
{
    std::string res = "";
    if(is_red)
    {
        for (size_t i = 0; i < Red_Controls_Button.size(); i++)
        {
            if (i%2 == 0)
            {
                res += Red_Controls_Button[i] + ": " + Controls_Direction[i] + '\n';
            }
            else
            {
                res += Red_Controls_Button[i] + ": " + Controls_Direction[i] + '\n';
            }
        }
    }
    else
    {
        for (size_t i = 0; i < Green_Controls_Button.size(); i++)
        {
            if (i%2 == 0)
            {
                res += Green_Controls_Button[i] + ": " + Controls_Direction[i] + '\n';
            }
            else
            {
                res += Green_Controls_Button[i] + ": " + Controls_Direction[i] + '\n';
            }
        }
    }
    return res;
}

void UI::DrawGlobalSettings(std::function<void()> fun)
{
    if (!m_show_settings)
    {
        return;
    }

    float display_width  = DisplaySizeX() * 0.8f;
    float display_height = DisplaySizeY() * 0.7f;
    float startx = DisplaySizeX() * 0.1f;
    float starty = DisplaySizeY() * 0.1f;
    ImGui::SetNextWindowPos(ImVec2(startx, starty));
    ImGui::SetNextWindowSize(ImVec2(display_width, display_height));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.75f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 20.0f));

    ImGui::Begin("Global Settings", nullptr, ImGuiWindowFlags_NoTitleBar);
    ImGui::SetWindowFontScale(1.5f);

    // ---- Header ----
    ImGui::TextUnformatted("Global Settings");

    std::string close{"Close Settings"};
    ImVec2 close_size = ImGui::CalcTextSize(close.c_str());
    close_size.x += 10;
    close_size.y += 10;
    ImGui::SameLine(display_width - close_size.x - style.padding.x * 2.0f);
    if (ImGui::Button(close.c_str(), close_size))
    {
        m_show_settings = false;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ---- Scrollable settings body ----
    ImGui::BeginChild("GlobalSettingsBody", ImVec2(0, 0), false);

    {
        ScopedFontScale headerFontScale(2.0f);
        ImGui::SetNextItemOpen(m_show_audio_settings);
        m_show_audio_settings = ImGui::CollapsingHeader("Audio Settings##audio");
    }
    if (m_show_audio_settings)
    {
        ImGui::Indent();
        float music_vol{MusicVol()};
        float sfx_vol{SFXVol()};
        float ui_vol{UIVol()};
        if (UIWidgets::Slider("   Music Sound", &music_vol, 0.0f, 1.0f, 1.5f, 200.0f))
        {
            SetMusicVol(music_vol);
        }
        ImGui::Spacing();
        if (UIWidgets::Slider("   SFX Sound", &sfx_vol, 0.0f, 1.0f, 1.5f, 200.0f))
        {
            SetSFXVol(sfx_vol);
        }
        ImGui::Spacing();
        if (UIWidgets::Slider("   UI Sound", &ui_vol, 0.0f, 1.0f, 1.5f, 200.0f))
        {
            SetUIVol(ui_vol);
        }
        ImGui::Spacing();
        ImGui::Unindent();
    }

    ImGui::Spacing();

    fun();

    ImGui::EndChild();

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}
