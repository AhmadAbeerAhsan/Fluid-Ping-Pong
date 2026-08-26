#include "UI.hpp"

UI::UI(GLFWwindow *window) :
    m_audio_player(),
    m_app_state()
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

void UI::BeginFullscreenOverlay(const char* name)
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(viewport->WorkSize, ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoSavedSettings;

    ImGui::Begin(name, nullptr, flags);
}

void UI::EndOverlay()
{
    ImGui::End();
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
                       const std::string& rightName, int rightScore)
{
    float display_width = DisplaySizeX();

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(display_width, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
    ImGui::Begin(
        "ScoreHUD", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav
    );

    ImGui::SetWindowFontScale(2.0f);

    std::string score_text = std::to_string(leftScore) + " : " + std::to_string(rightScore);

    ImVec2 leftNameSize = ImGui::CalcTextSize(leftName.c_str());
    ImVec2 scoreSize    = ImGui::CalcTextSize(score_text.c_str());

    float leftNameX  = display_width * 0.38f - leftNameSize.x; // ends near 38%, biased toward center
    float rightNameX = display_width * 0.62f;                   // mirrored start
    float scoreX     = display_width * 0.5f - scoreSize.x * 0.5f;

    ImGui::SetCursorPosX(leftNameX);
    ImGui::Text("%s", leftName.c_str());

    ImGui::SameLine(scoreX);
    ImGui::Text("%s", score_text.c_str());

    ImGui::SameLine(rightNameX);
    ImGui::Text("%s", rightName.c_str());

    ImGui::SetWindowFontScale(1.0f);

    ImGui::End();
    ImGui::PopStyleColor();
}