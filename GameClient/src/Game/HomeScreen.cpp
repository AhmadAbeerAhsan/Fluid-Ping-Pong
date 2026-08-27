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
}

HomeScreen::~HomeScreen()
{
    std::cout << "HomeScreen::~HomeScreen()" << std::endl;
}

void HomeScreen::SetupUI()
{
    GameScreen::SetupUI();

    if(!m_ui->m_show_settings)
    {
        if(m_show_matchmaking_menu)
        {
            DrawOnlineMatchMakingMenu();
        }
        else
        {
            DrawMenu();
        }
    }
    m_ui->DrawGlobalSettings();
}

void HomeScreen::DrawMenu()
{
    float display_width = m_ui->DisplaySizeX() * 0.8f;
    float display_height = m_ui->DisplaySizeY() * 0.7;
    float startx = m_ui->DisplaySizeX() * 0.1f;
    float starty = m_ui->DisplaySizeY() * 0.1f;
    ImGui::SetNextWindowPos(ImVec2(startx, starty));
    ImGui::SetNextWindowSize(ImVec2(display_width, display_height));

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoSavedSettings;

    ImGui::Begin("HomeScreen", nullptr, flags);

    ImGui::SetCursorPosY((ImGui::GetWindowHeight() - 380.0f) * 0.5f);
    UIWidgets::Label("SPACE HOCKEY", 2.5f, UIWidgets::HorizontalLayout::Middle);

    ImGui::Spacing();
    ImGui::Spacing();

    UIWidgets::InputField("Username:", m_player_name, sizeof(m_player_name), 1.5f, UIWidgets::HorizontalLayout::Middle);

    ImGui::Spacing();

    if (UIWidgets::Button("Human vs Computer", 1.5f, UIWidgets::HorizontalLayout::Middle))
    {
        m_ui->Username = m_player_name;
        // InitMatch(GameMode::Bot);
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    if (UIWidgets::Button("Human vs Human", 1.5f, UIWidgets::HorizontalLayout::Middle))
    {
        m_ui->Username = m_player_name;
        p1 = Controller::PlayerType::Red; p2 = Controller::PlayerType::Green;
        c1 = Controller::ControllerType::Keyboard; c2 = Controller::ControllerType::Keyboard;
        m_ui->m_match_requested = true;
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    if (UIWidgets::Button("Show Online Matchmaking", 1.5f, UIWidgets::HorizontalLayout::Middle))
    {
        m_show_matchmaking_menu = true;
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    if (UIWidgets::Button("Settings", 1.5f, UIWidgets::HorizontalLayout::Middle))
    {
        m_ui->m_show_settings = true;
    }

    ImGui::End();
    
}

void HomeScreen::DrawOnlineMatchMakingMenu()
{
    float display_width = m_ui->DisplaySizeX() * 0.7f;
    float display_height = m_ui->DisplaySizeY() * 0.7;
    float startx = m_ui->DisplaySizeX() * 0.15f;
    float starty = m_ui->DisplaySizeY() * 0.15f;
    ImGui::SetNextWindowPos(ImVec2(startx, starty));
    ImGui::SetNextWindowSize(ImVec2(display_width, display_height));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
    ImGui::Begin(
        "Online Matchmaking -------------", nullptr, ImGuiWindowFlags_NoTitleBar
    );

    ImGui::SetWindowFontScale(1.5f);
    std::string back{"Back To Menu"};
    ImVec2 back_size = ImGui::CalcTextSize(back.c_str());
    back_size.x += 10;
    back_size.y += 10;
    ImGui::SetCursorPosX(display_width - back_size.x);
    if (ImGui::Button(back.c_str(), back_size))
    {
        m_show_matchmaking_menu = false;
    }

    UIWidgets::InputField("      Match Name:", m_player_name, sizeof(m_player_name), 1.5f, UIWidgets::HorizontalLayout::Left);
    ImGui::SameLine();
    if (UIWidgets::Button("Create Match", 1.5f, UIWidgets::HorizontalLayout::Middle))
    {
        
    }

    for (size_t i = 0; i < 3; i++)
    {
        ImGui::PushID(static_cast<int>(i));
        UIWidgets::OnlineMatch("Terri", "1", [](){}, 1.5f);
        ImGui::Spacing();
        ImGui::PopID();
    }
    

    ImGui::End();
    ImGui::PopStyleColor(); 
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
    if (m_ui->m_match_requested)
    {
        m_ui->m_match_requested = false;
        m_ui->Navigate_To_Match(p1, p2, c1, c2);
    }
    
}
