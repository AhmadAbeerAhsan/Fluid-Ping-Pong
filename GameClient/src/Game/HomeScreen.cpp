#include "HomeScreen.hpp"

HomeScreen::HomeScreen(
    std::shared_ptr<glm::ivec2>& shared_resolution,
    std::shared_ptr<UI>& ui_ptr,
    std::string texture_path
) :
    GameScreen(shared_resolution, ui_ptr),
    m_texture(texture_path.c_str())
{
    std::cout << "Creating shader m_screen_texture_shader..." << std::endl;
    m_screen_texture_shader = Shader{"GameClient/src/Renderer/Shaders/screen_texture.vs.glsl", "GameClient/src/Renderer/Shaders/screen_texture.fs.glsl"};
    std::cout << "m_screen_texture_shader id: " << *m_screen_texture_shader.ID << std::endl;

    std::vector<glm::vec3> m_positions {};
    std::vector<glm::vec3> m_colors {};
    std::vector<glm::uvec3> m_indices {};
    std::vector<glm::vec2> m_tex_coords {};
    GenerateFullscreenQuad(m_positions, m_tex_coords, m_indices);
    fullscreen_quad.SetGeometry(m_positions, m_indices);
    fullscreen_quad.SetMaterial(m_texture, m_tex_coords);
    fullscreen_quad.SetShader(m_screen_texture_shader);
    fullscreen_quad.initializeForGL();
    fullscreen_quad.UpdateModelMatrix();

    m_ui_style.fontScale = 2.5f;
}

HomeScreen::~HomeScreen()
{
    std::cout << "HomeScreen::~HomeScreen()" << std::endl;
}

void HomeScreen::SetupUI()
{
    GameScreen::SetupUI();

    m_ui_ptr->DrawGlobalSettings();
    /*
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(viewport->WorkSize, ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoSavedSettings;

    ImGui::Begin("HomeScreen", nullptr, flags);

    const float menuWidth = 300.0f;
    const float buttonHeight = 50.0f;
    const float spacing = 12.0f;

    ImGui::SetCursorPosY((ImGui::GetWindowHeight() - 380.0f) * 0.5f);
    m_ui_style.fontScale = 1.5f;
    UIWidgets::Label("SPACE HOCKEY", m_ui_style, UIWidgets::HorizontalLayout::Middle);

    ImGui::Spacing();
    ImGui::Spacing();

    m_ui_style.fontScale = 1.5f;
    UIWidgets::InputField("Username:", m_player_name, sizeof(m_player_name), m_ui_style, UIWidgets::HorizontalLayout::Middle);

    ImGui::Spacing();

    if (UIWidgets::Button("Human vs Bot", m_ui_style, ImVec2(menuWidth, buttonHeight), UIWidgets::HorizontalLayout::Middle))
    {
        m_ui_ptr->SetUsername(m_player_name);
        // InitMatch(GameMode::Bot);
    }
    ImGui::Dummy(ImVec2(0.0f, spacing));

    if (UIWidgets::Button("Human vs Human", m_ui_style, ImVec2(menuWidth, buttonHeight), UIWidgets::HorizontalLayout::Middle))
    {
        m_ui_ptr->SetUsername(m_player_name);
        m_ui_ptr->Navigate_To_Match(0, 1);
    }
    ImGui::Dummy(ImVec2(0.0f, spacing));

    if (UIWidgets::Button("Human vs Online Human", m_ui_style, ImVec2(menuWidth, buttonHeight), UIWidgets::HorizontalLayout::Middle))
    {
        // Open online matchmaking
    }
    ImGui::Dummy(ImVec2(0.0f, spacing));

    if (UIWidgets::Button("Settings", m_ui_style, ImVec2(menuWidth, buttonHeight), UIWidgets::HorizontalLayout::Middle))
    {
        // Open settings
    }

    ImGui::End();
    */
}

void HomeScreen::DrawScene()
{
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_displayBuffer.Bind();

    m_screen_texture_shader.Activate();
    fullscreen_quad.DrawWithExternalShader(m_screen_texture_shader);

    m_displayBuffer.Unbind();
}

void HomeScreen::OnChangeResolution()
{
    GameScreen::OnChangeResolution();
}

void HomeScreen::OnMouseMoved(GLFWwindow *window_ptr, double xposIn, double yposIn)
{
}

void HomeScreen::OnKeyPressed(GLFWwindow *window_ptr)
{
    std::cout << "HomeScreen::OnKeyPressed(GLFWwindow *window_ptr)" << std::endl;
}

void HomeScreen::ListenKeysPressed()
{
}

void HomeScreen::ProcessPendingNavigation()
{
}
