#include "HomeScreen.hpp"

void HomeScreen::RefreshOnlineGameSessionList()
{
    game_sessions.clear();
    std::shared_ptr<std::string> connect_message{std::make_shared<std::string>(
        std::format(
            "{}{}",
            contract(Action::ListSessions),contract(Action::EndDeliminator)
        )
    )};
    m_con->tcpC.Send(connect_message, true);
}

void HomeScreen::SendJoinReq(int match_id, int player_type)
{
    std::cout << std::format(
        "MatchId: {}, JoiningId:{}\n", match_id, joining_session.MatchId()
    );
    std::shared_ptr<std::string> connect_message{std::make_shared<std::string>(
        std::format(
            "{}{}{}{}{}{}",
            contract(Action::Join), contract(Action::Deliminator),
            player_type,            contract(Action::Deliminator),
            match_id,               contract(Action::EndDeliminator)
        )
    )};
    m_con->udpC.StartSend(connect_message);
}

void HomeScreen::InitScene()
{
    m_camera_ptr = std::make_shared<Camera>(
        100.0f, 65.0f, 90.0f,
        m_shared_resolution,
        45.0f,
        0.01f, 1000.0f
    );
    m_camera_ptr->updatePersprectiveProj();
    //Create Textures
    Texture wall_texture{"GameClient/assets/textures/tile.jpg"};
    Texture floor_texture{"GameClient/assets/textures/base.png"};
    Texture ball_texture{"GameClient/assets/textures/ball.png"};
    std::vector<std::string> cubemap_paths{
        "GameClient/assets/textures/cube-space/blue_nebula.jpeg",
        "GameClient/assets/textures/cube-space/blue_nebula.jpeg",
        "GameClient/assets/textures/cube-space/blue_nebula.jpeg",
        "GameClient/assets/textures/cube-space/blue_nebula.jpeg",
        "GameClient/assets/textures/cube-space/blue_nebula.jpeg",
        "GameClient/assets/textures/cube-space/blue_nebula.jpeg"
    };
    m_cube_map_texture = Texture{cubemap_paths};

    std::cout << "Creating shader m_texture_cubemap_shader..." << std::endl;
    m_texture_cubemap_shader = Shader{"GameClient/src/Renderer/Shaders/texture_cubemap.vs.glsl", "GameClient/src/Renderer/Shaders/texture_cubemap.fs.glsl"};
    std::cout << "m_texture_cubemap_shader id: " << *m_texture_cubemap_shader.ID << std::endl;
    m_texture_cubemap_shader.PassUniforms = std::function<void()>{
        [this](){
            m_texture_cubemap_shader.setMat4("view", m_camera_ptr->skyboxView);
            m_texture_cubemap_shader.setMat4("projection", m_camera_ptr->proj);
        }
    };

    std::vector<glm::vec3> m_positions {};
    std::vector<glm::vec3> m_colors {};
    std::vector<glm::uvec3> m_indices {};
    std::vector<glm::vec2> m_tex_coords {};

    std::cout << "Generating cube map" << std::endl;
    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateSkyboxCube(m_positions, m_tex_coords, m_indices);
    m_cube_skybox.SetGeometry(m_positions, m_indices, false);
    m_cube_skybox.SetMaterial(m_cube_map_texture, m_tex_coords);
    m_cube_skybox.SetShader(m_texture_cubemap_shader);
    m_cube_skybox.initializeForGL();
}

HomeScreen::HomeScreen(
    std::shared_ptr<glm::ivec2> &shared_resolution,
    std::shared_ptr<UI> &ui_ptr,
    std::shared_ptr<Connection> &con,
    std::string texture_path) : GameScreen(shared_resolution, ui_ptr, con),
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

    InitScene();
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
    float display_width  = m_ui->DisplaySizeX() * 0.8f;
    float display_height = m_ui->DisplaySizeY() * 0.7f;
    float startx = m_ui->DisplaySizeX() * 0.1f;
    float starty = m_ui->DisplaySizeY() * 0.1f;
    ImGui::SetNextWindowPos(ImVec2(startx, starty));
    ImGui::SetNextWindowSize(ImVec2(display_width, display_height));

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoSavedSettings;

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.55f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);

    ImGui::Begin("HomeScreen", nullptr, flags);

    // ---- Title block ----
    ImGui::SetCursorPosY((ImGui::GetWindowHeight() - 380.0f) * 0.5f);
    UIWidgets::Label("SPACE HOCKEY", 3.5f, UIWidgets::HorizontalLayout::Middle);
    UIWidgets::Label("A fast-paced arcade showdown", 2.2f, UIWidgets::HorizontalLayout::Middle);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    // ---- Menu buttons ----
    if (UIWidgets::Button("Human vs Human", 2.0f, UIWidgets::HorizontalLayout::Middle))
    {
        m_ui->Username = m_player_name;
        c1 = Controller::ControllerType::Keyboard1; c2 = Controller::ControllerType::Keyboard2;
        m_match_type = Match::MatchType::Offline;
        m_ui->m_match_requested = true;
    }
    ImGui::Dummy(ImVec2(0.0f, 14.0f));

    // ---- Menu buttons ----
    if (UIWidgets::Button("Human vs Bot", 2.0f, UIWidgets::HorizontalLayout::Middle))
    {
        m_ui->Username = m_player_name;
        c1 = Controller::ControllerType::Keyboard1; c2 = Controller::ControllerType::Bot;
        m_match_type = Match::MatchType::Offline;
        m_ui->m_match_requested = true;
    }
    ImGui::Dummy(ImVec2(0.0f, 14.0f));

    if (UIWidgets::Button("Show Online Matchmaking", 2.0f, UIWidgets::HorizontalLayout::Middle))
    {
        m_show_matchmaking_menu = true;
        RefreshOnlineGameSessionList();
    }
    ImGui::Dummy(ImVec2(0.0f, 14.0f));

    if (UIWidgets::Button("Settings", 2.0f, UIWidgets::HorizontalLayout::Middle))
    {
        m_ui->m_show_settings = true;
    }

    ImGui::End();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

void HomeScreen::DrawOnlineMatchMakingMenu()
{
    float display_width  = m_ui->DisplaySizeX() * 0.7f;
    float display_height = m_ui->DisplaySizeY() * 0.7f;
    float startx = m_ui->DisplaySizeX() * 0.15f;
    float starty = m_ui->DisplaySizeY() * 0.15f;

    ImGui::SetNextWindowPos(ImVec2(startx, starty));
    ImGui::SetNextWindowSize(ImVec2(display_width, display_height));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.75f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 20.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);

    ImGui::Begin("Online Matchmaking", nullptr, ImGuiWindowFlags_NoTitleBar);
    ImGui::SetWindowFontScale(1.5f);

    float content_width = ImGui::GetContentRegionAvail().x;

    // ---- Header row: title (left), refresh + back (right) ----
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Online Matchmaking");

    const float button_pad = 10.0f;
    ImVec2 back_size    = ImGui::CalcTextSize("Back To Menu");
    back_size.x    += button_pad * 2.0f;
    back_size.y    += button_pad;
    ImVec2 refresh_size = ImGui::CalcTextSize("Refresh List");
    refresh_size.x += button_pad * 2.0f;
    refresh_size.y += button_pad;

    float header_buttons_width = refresh_size.x + back_size.x + style.padding.x;
    ImGui::SameLine(content_width - header_buttons_width);

    if (ImGui::Button("Refresh List", refresh_size))
    {
        RefreshOnlineGameSessionList();
    }
    ImGui::SameLine();
    if (ImGui::Button("Back To Menu", back_size))
    {
        m_show_matchmaking_menu = false;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ---- Create match row ----
    ImGui::TextDisabled("Create New Match");
    ImGui::Spacing();

    UIWidgets::InputField("      Match Name:", m_match_name, sizeof(m_match_name), 1.5f, UIWidgets::HorizontalLayout::Left);
    ImGui::SameLine();
    if (UIWidgets::Button("Create Match", 1.5f, UIWidgets::HorizontalLayout::Middle))
    {
        std::string name{m_match_name};
        if (!name.empty())
        {
            std::shared_ptr<std::string> connect_message{std::make_shared<std::string>(
                std::format(
                    "{}{}{}{}",
                    contract(Action::Create), contract(Action::Deliminator),
                    name,                      contract(Action::EndDeliminator)
                )
            )};
            m_con->tcpC.Send(connect_message, true);
        }
        else
        {
            std::cout << "Create Match: name is empty" << std::endl;
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // ---- Session list ----
    ImGui::TextDisabled("Available Matches");
    ImGui::Spacing();

    ImGui::BeginChild("SessionList", ImVec2(0, 0), false);

    if (game_sessions.empty())
    {
        ImGui::Spacing();
        ImGui::TextDisabled("No matches found. Try refreshing or create one above.");
    }
    else
    {
        for (size_t i = 0; i < game_sessions.size(); i++)
        {
            ImGui::PushID(static_cast<int>(i));
            UIWidgets::OnlineMatch(
                game_sessions[i],
                [this, match_id = game_sessions[i].MatchId()](int player_type){
                    joining_session.match_id = match_id;
                    SendJoinReq(match_id, player_type);
                },
                1.5f
            );
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::PopID();
        }
    }

    ImGui::EndChild();

    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

void HomeScreen::DrawScene()
{
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_displayBuffer.Bind();

    glDisable(GL_DEPTH_TEST);
    m_texture_cubemap_shader.Activate();
    m_texture_cubemap_shader.PassUniforms();
    m_cube_skybox.DrawWithExternalShader(m_texture_cubemap_shader);
    glEnable(GL_DEPTH_TEST);

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
    GameSessionData new_game_session_data;
    while (m_con->game_sessions.Read(new_game_session_data))
    {
        std::cout << std::format(
            "{} {}\n", "Read From Window:", new_game_session_data.EncodeBuffer()
        );
        game_sessions.push_back(new_game_session_data);
    }

    GameEventData new_game_event_data;
    while (m_con->game_events.Read(new_game_event_data))
    {
        std::cout << std::format(
            "{} {}\n", "Read From Window:", new_game_event_data.EncodeBuffer()
        );
        std::cout << std::format(
            "MatchId: {}, JoiningId:{}\n", new_game_event_data.m_match_id, joining_session.MatchId()
        );
        if (new_game_event_data.m_match_id == joining_session.MatchId())
        {
            if (new_game_event_data.m_player_type == GameEventData::ObjectType::Red)
            {
                c1 = Controller::ControllerType::Keyboard1; c2 = Controller::ControllerType::Online;
                m_match_type = Match::MatchType::Online;
                m_ui->m_match_requested = true;
            }
            else if (new_game_event_data.m_player_type == GameEventData::ObjectType::Green)
            {
                c1 = Controller::ControllerType::Online; c2 = Controller::ControllerType::Keyboard1;
                m_match_type = Match::MatchType::Online;
                m_ui->m_match_requested = true;
            } 
        }
        
    }

    ErrorData e;
    while (m_con->error_messages.Read(e))
    {
        std::cout << std::format(
            "{} {}\n", "Read From Window:", e.EncodeBuffer()
        );
    }
}

void HomeScreen::ProcessPendingNavigation()
{
    if (m_ui->m_match_requested)
    {
        m_ui->m_match_requested = false;
        m_ui->Navigate_To_Match(c1, c2, m_match_type, joining_session);
    }
}

void HomeScreen::Clean()
{
}
