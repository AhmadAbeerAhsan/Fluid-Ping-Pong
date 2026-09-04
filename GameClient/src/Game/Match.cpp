#include "Match.hpp"

Match::Match(
    Controller::ControllerType player1_controller, Controller::ControllerType player2_controller,
    std::shared_ptr<glm::ivec2>& shared_resolution,
    std::shared_ptr<UI>& ui_ptr,
    std::shared_ptr<Connection>& con,
    MatchType match_type,
    GameSessionData game_session_data
) : GameScreen(shared_resolution, ui_ptr, con),
    m_player_red(GameEventData::ObjectType::Red, player1_controller),
    m_player_green(GameEventData::ObjectType::Green, player2_controller),
    old_red_keyboard(player1_controller),
    old_green_keyboard(player2_controller),
    m_snapshotBuffer(Framebuffer::FrameBufferType::Color_FloatAlpha, shared_resolution),
    m_pointLight(m_shared_resolution),
    m_collision_engine{ui_ptr},
    m_generator{std::random_device{}()},
    m_game_session_data(game_session_data),
    m_match_type(match_type)
{
    InitScene();
    InitializePassInputs();
    SetUpCollisionEngine();
    start = std::chrono::steady_clock::now();
    m_last_online_event_recieved_time = std::chrono::steady_clock::now();
}


Match::~Match()
{
    Clean();
    std::cout << "Match::~Match()" << std::endl;
}

void Match::ReadOnlineEvents()
{
    GameEventData new_event_data;
    while (m_con->game_events.Read(new_event_data))
    {
        m_last_online_event_recieved_time = std::chrono::steady_clock::now();
        if (new_event_data.m_player_type == m_local_player_type)
        {
            ProcessLocalPlayerEvents(new_event_data);
        }
        else if (new_event_data.m_player_type == m_online_player_type)
        {
            ProcessOnlinePlayerEvents(new_event_data);
        }
        else if (new_event_data.m_player_type == GameEventData::ObjectType::Ball)
        {
            ProcessRecievedBallEvents(new_event_data);
        }    
    }
}

void Match::ProcessLocalPlayerEvents(GameEventData &e)
{
    std::cout << std::format(
        "{} {}\n", "Read Local Player Window:", e.EncodeBuffer()
    );
    now = std::chrono::steady_clock::now();
    int time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
    m_local_game_event_window.UpdateLag(e, time_elapsed);
}

void Match::ProcessOnlinePlayerEvents(GameEventData &e)
{
    std::cout << std::format(
        "{} {}\n", "Read Online Player Window:", e.EncodeBuffer()
    );
    if (IsIncomingEventNewer( m_last_opponent_game_event, e ))
    {
        UpdateScoreOnline(e);

        m_last_opponent_game_event = e;
        online_inputs[0].x = (float)m_last_opponent_game_event.m_player_pos_x / GameEventData::factor;
        online_inputs[0].y = (float)m_last_opponent_game_event.m_player_pos_z / GameEventData::factor;

        online_inputs[1].x = (float)m_last_opponent_game_event.m_player_vel_x / GameEventData::factor;
        online_inputs[1].y = (float)m_last_opponent_game_event.m_player_vel_z / GameEventData::factor;
        
        online_inputs[2].x = (float)m_last_opponent_game_event.m_lag_ms / GameEventData::factor;
        online_inputs[2].y = (float)m_local_game_event_window.Lag() / GameEventData::factor;
    }
    DetermineWinner();
}

void Match::ProcessRecievedBallEvents(GameEventData &e)
{
    std::cout << std::format(
        "{} {}\n", "Read Online Ball Window:", e.EncodeBuffer()
    );
    if (IsIncomingEventNewer( m_last_recieved_ball_game_event, e ))
    {
        glm::vec2 new_pos = glm::vec2(e.m_player_pos_x, e.m_player_pos_z)/GameEventData::factor;
        if(glm::dot(new_pos, new_pos) > 0.01f && !DetermineWinner())
            m_match_running = true;

        if (!IsBallInOnlineSide(new_pos.y))
        {
            std::cout << std::format(
                "{} Recieved: {}, Old: {}\n", "Ball is in local Pos:", e.EncodeBuffer(), m_last_recieved_ball_game_event.EncodeBuffer()
            );
            return;
        }

        m_last_recieved_ball_game_event = e;
        
        m_boundary_ball_ptr->SetVelocity(
            glm::vec2(
                (float)m_last_recieved_ball_game_event.m_player_vel_x / GameEventData::factor,
                (float)m_last_recieved_ball_game_event.m_player_vel_z / GameEventData::factor
            )
        );

        m_boundary_ball_ptr->SetPosition(
            new_pos +
            (
                (float)(m_local_game_event_window.Lag() + m_last_recieved_ball_game_event.m_lag_ms)/GameEventData::factor *
                m_boundary_ball_ptr->Velocity()
            ),
            ball_interpolation_duration_short_ms
        );
    }
    else
    {
        std::cout << std::format(
            "{} Recieved: {}, Old: {}\n", "Ball Outdated:", e.EncodeBuffer(), m_last_recieved_ball_game_event.EncodeBuffer()
        );
    }
}

void Match::InitScene()
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
        "GameClient/assets/textures/cube_maps/px.png",
        "GameClient/assets/textures/cube_maps/nx.png",
        "GameClient/assets/textures/cube_maps/py.png",
        "GameClient/assets/textures/cube_maps/ny.png",
        "GameClient/assets/textures/cube_maps/pz.png",
        "GameClient/assets/textures/cube_maps/nz.png"
    };
    m_cube_map_texture = Texture{cubemap_paths};

    std::cout << "Creating shader m_blinn_phong_shdader..." << std::endl;
    m_blinn_phong_shdader = Shader{"GameClient/src/Renderer/Shaders/blinn_phong.vs.glsl", "GameClient/src/Renderer/Shaders/blinn_phong.fs.glsl"};
    std::cout << "m_blinn_phong_shdader id: " << *m_blinn_phong_shdader.ID << std::endl;
    m_blinn_phong_shdader.Activate();
    m_blinn_phong_shdader.setInt("ourTexture", 0);
    m_blinn_phong_shdader.setInt("shTex", 1);
    m_blinn_phong_shdader.setInt("skybox", 2);
    m_blinn_phong_shdader.setInt("scene", 3);
    m_blinn_phong_shdader.PassUniforms = std::function<void()>{
        [this](){
            m_blinn_phong_shdader.setMat4("view", m_camera_ptr->view);
            m_blinn_phong_shdader.setMat4("projection", m_camera_ptr->proj);
            m_blinn_phong_shdader.setVec3("cameraPosition", m_camera_ptr->position);
            m_blinn_phong_shdader.setVec2("resolution", {(float)m_shared_resolution->x, (float)m_shared_resolution->y});
            m_pointLight.PassUniformsToRendererShader();
            m_pointLight.EnableShadowTexture();
            m_cube_map_texture.Use(GL_TEXTURE2);
        }
    };

    std::cout << "Creating shader m_shadow_map_shdader..." << std::endl;
    m_shadow_map_shdader = Shader{"GameClient/src/Renderer/Shaders/shadow_map.vs.glsl", "GameClient/src/Renderer/Shaders/shadow_map.fs.glsl"};
    std::cout << "m_shadow_map_shdader id: " << *m_shadow_map_shdader.ID << std::endl;

    std::cout << "Creating shader m_texture_cubemap_shdader..." << std::endl;
    m_texture_cubemap_shdader = Shader{"GameClient/src/Renderer/Shaders/texture_cubemap.vs.glsl", "GameClient/src/Renderer/Shaders/texture_cubemap.fs.glsl"};
    std::cout << "m_texture_cubemap_shdader id: " << *m_texture_cubemap_shdader.ID << std::endl;
    m_texture_cubemap_shdader.PassUniforms = std::function<void()>{
        [this](){
            m_texture_cubemap_shdader.setMat4("view", m_camera_ptr->skyboxView);
            m_texture_cubemap_shdader.setMat4("projection", m_camera_ptr->proj);
        }
    };

    m_pointLight.SetRenderShader(m_blinn_phong_shdader);
    m_pointLight.SetShadowMapShader(m_shadow_map_shdader);

    m_red_spawn_points = std::vector<glm::vec2>{
        glm::vec2(width/4.0f, lenght/4.0f),
        glm::vec2(-width/4.0f, lenght/4.0f),
    };
    m_green_spawn_points = std::vector<glm::vec2>{
        glm::vec2(width/4.0f, -lenght/4.0f),
        glm::vec2(-width/4.0f, -lenght/4.0f),
    };

    std::vector<glm::vec3> m_positions {};
    std::vector<glm::vec3> m_colors {};
    std::vector<glm::uvec3> m_indices {};
    std::vector<glm::vec2> m_tex_coords {};

    float diagnal_length{CalculateDiagonalLength(
        (width - goal_lenght)/2.0f,
        (lenght - side_border_lenght)/2.0f
    )};


    //right_border
    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, min_size, min_size, side_border_lenght);
    std::shared_ptr<Model> right_border{std::make_shared<Model>()};
    m_models.emplace_back(right_border);
    m_models.back()->SetGeometry(m_positions, m_indices);
    m_models.back()->SetMaterial(wall_texture, m_tex_coords);
    m_models.back()->SetShader(m_blinn_phong_shdader);
    m_models.back()->initializeForGL();
    m_models.back()->Translate(glm::vec3(-1.0f * (width/2.0f + min_size/2.0f), 0, 0));
    m_models.back()->UpdateModelMatrix();

    //left_border
    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, min_size, min_size, side_border_lenght);
    std::shared_ptr<Model> left_border{std::make_shared<Model>()};
    m_models.emplace_back(left_border);
    m_models.back()->SetGeometry(m_positions, m_indices);
    m_models.back()->SetMaterial(wall_texture, m_tex_coords);
    m_models.back()->SetShader(m_blinn_phong_shdader);
    m_models.back()->initializeForGL();
    m_models.back()->Translate(glm::vec3(1.0f * (width/2.0f + min_size/2.0f), 0, 0));
    m_models.back()->UpdateModelMatrix();

    glm::vec3 translation;
    glm::vec3 rev_nor_translation;
    glm::vec3 final_translation;

    //bottom_left_border
    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, diagnal_length, min_size, min_size);
    std::shared_ptr<Model> bottom_left_border{std::make_shared<Model>()};
    m_models.emplace_back(bottom_left_border);
    m_models.back()->SetGeometry(m_positions, m_indices);
    m_models.back()->SetMaterial(wall_texture, m_tex_coords);
    m_models.back()->SetShader(m_blinn_phong_shdader);
    m_models.back()->initializeForGL();
    m_models.back()->RotateY(glm::radians(-39.806f));
    m_models.back()->Translate(glm::vec3(goal_lenght/2.0f, 0, -(lenght/2.0f)));
    translation = glm::vec3((width - goal_lenght)/2.0f, 0.0f, (lenght - side_border_lenght)/2.0f);
    translation = translation/2.0f;
    rev_nor_translation = glm::normalize(glm::vec3(translation.z, translation.y, -translation.x));
    final_translation = translation + (rev_nor_translation * min_size/2.0f);
    m_models.back()->Translate(final_translation);
    m_models.back()->UpdateModelMatrix();

    //bottom_right_border
    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, diagnal_length, min_size, min_size);
    std::shared_ptr<Model> bottom_right_border{std::make_shared<Model>()};
    m_models.emplace_back(bottom_right_border);
    m_models.back()->SetGeometry(m_positions, m_indices);
    m_models.back()->SetMaterial(wall_texture, m_tex_coords);
    m_models.back()->SetShader(m_blinn_phong_shdader);
    m_models.back()->initializeForGL();
    m_models.back()->RotateY(glm::radians(39.806f));
    m_models.back()->Translate(glm::vec3(-goal_lenght/2.0f, 0, -(lenght/2.0f)));
    translation = glm::vec3(-(width - goal_lenght)/2.0f, 0.0f, (lenght - side_border_lenght)/2.0f);
    translation = translation/2.0f;
    rev_nor_translation = glm::normalize(glm::vec3(translation.z, translation.y, -translation.x));
    final_translation = translation - (rev_nor_translation * min_size/2.0f);
    m_models.back()->Translate(final_translation);
    m_models.back()->UpdateModelMatrix();

    /*
    //bottom_goal_border
    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, goal_lenght, min_size, min_size);
    std::shared_ptr<Model> bottom_goal_border{std::make_shared<Model>()};
    m_models.emplace_back(bottom_goal_border);
    m_models.back()->SetGeometry(m_positions, m_indices);
    m_models.back()->SetMaterial(wall_texture, m_tex_coords);
    m_models.back()->SetShader(m_blinn_phong_shdader_ptr);
    m_models.back()->initializeForGL();
    m_models.back()->Translate(glm::vec3(0, 0, -(lenght + min_size)/2.0f));
    m_models.back()->UpdateModelMatrix();
    */
   
    //top_left_border
    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, diagnal_length, min_size, min_size);
    std::shared_ptr<Model> top_left_border{std::make_shared<Model>()};
    m_models.emplace_back(top_left_border);
    m_models.back()->SetGeometry(m_positions, m_indices);
    m_models.back()->SetMaterial(wall_texture, m_tex_coords);
    m_models.back()->SetShader(m_blinn_phong_shdader);
    m_models.back()->initializeForGL();
    m_models.back()->RotateY(glm::radians(39.806f));
    m_models.back()->Translate(glm::vec3(goal_lenght/2.0f, 0, (lenght/2.0f)));
    translation = glm::vec3((width - goal_lenght)/2.0f, 0.0f, -(lenght - side_border_lenght)/2.0f);
    translation = translation/2.0f;
    rev_nor_translation = glm::normalize(glm::vec3(translation.z, translation.y, -translation.x));
    final_translation = translation - (rev_nor_translation * min_size/2.0f);
    m_models.back()->Translate(final_translation);
    m_models.back()->UpdateModelMatrix();

    //top_right_border
    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, diagnal_length, min_size, min_size);
    std::shared_ptr<Model> top_right_border{std::make_shared<Model>()};
    m_models.emplace_back(top_right_border);
    m_models.back()->SetGeometry(m_positions, m_indices);
    m_models.back()->SetMaterial(wall_texture, m_tex_coords);
    m_models.back()->SetShader(m_blinn_phong_shdader);
    m_models.back()->initializeForGL();
    m_models.back()->RotateY(glm::radians(-39.806f));
    m_models.back()->Translate(glm::vec3(-goal_lenght/2.0f, 0, (lenght/2.0f)));
    translation = glm::vec3(-(width - goal_lenght)/2.0f, 0.0f, -(lenght - side_border_lenght)/2.0f);
    translation = translation/2.0f;
    rev_nor_translation = glm::normalize(glm::vec3(translation.z, translation.y, -translation.x));
    final_translation = translation + (rev_nor_translation * min_size/2.0f);
    m_models.back()->Translate(final_translation);
    m_models.back()->UpdateModelMatrix();

    
    /*
    //top_goal_border
    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, goal_lenght, min_size, min_size);
    m_models.emplace_back(std::make_shared<Model>();
    m_models.back()->SetGeometry(m_positions, m_indices);
    m_models.back()->SetMaterial(wall_texture, m_tex_coords);
    m_models.back()->SetShader(m_blinn_phong_shdader_ptr);
    m_models.back()->initializeForGL();
    m_models.back()->Translate(glm::vec3(0, 0, (lenght + min_size)/2.0f));
    m_models.back()->UpdateModelMatrix();
    */

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateXZBase(m_positions, m_colors, m_indices, m_tex_coords, width, lenght, goal_lenght, side_border_lenght);
    m_floor.SetGeometry(m_positions, m_indices);
    m_floor.SetMaterial(floor_texture, m_tex_coords);
    m_floor.m_enable_reflection = true;
    m_floor.SetShader(m_blinn_phong_shdader);
    m_floor.initializeForGL();
    m_floor.Translate(glm::vec3(0, -min_size/2.0f, 0));
    m_floor.RotateY(0.0f);
    m_floor.UpdateModelMatrix();
    //m_models.emplace_back(m_floor);


    float r1{1.0f}, b1{2.0f}, b2{0.5f};
    //handle
    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateHandle(
        m_positions, m_colors, m_indices, m_tex_coords,
        r1,
        3.0f, 1.0f, 1.0f,
        b1, b2,
        10,
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    );
    std::shared_ptr<Model> handle_red{std::make_shared<Model>()};
    m_models.emplace_back(handle_red);
    m_models.back()->SetGeometry(m_positions, m_indices);
    m_models.back()->SetMaterial(m_colors);
    m_models.back()->SetShader(m_blinn_phong_shdader);
    m_models.back()->initializeForGL();
    m_models.back()->Translate(glm::vec3(0, -min_size/2.0f, 30.0f));
    m_models.back()->UpdateModelMatrix();

    //handle2
    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateHandle(
        m_positions, m_colors, m_indices, m_tex_coords,
        r1,
        3.0f, 1.0f, 1.0f,
        b1, b2,
        10,
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    );
    std::shared_ptr<Model> handle_green{std::make_shared<Model>()};
    m_models.emplace_back(handle_green);
    m_models.back()->SetGeometry(m_positions, m_indices);
    m_models.back()->SetMaterial(m_colors);
    m_models.back()->SetShader(m_blinn_phong_shdader);
    m_models.back()->initializeForGL();
    m_models.back()->Translate(glm::vec3(0, -min_size/2.0f, -30.0f));
    m_models.back()->UpdateModelMatrix();
    
    m_handle_radius = r1 + b1 + b2;

    //sphere
    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateSphere(m_positions, m_colors, m_indices, m_tex_coords, 16, m_ball_radius);
    std::shared_ptr<Model> sphere{std::make_shared<Model>()};
    m_models.emplace_back(sphere);
    m_models.back()->SetGeometry(m_positions, m_indices, true);
    m_models.back()->SetMaterial(ball_texture, m_tex_coords);
    m_models.back()->SetShader(m_blinn_phong_shdader);
    m_models.back()->initializeForGL();
    m_models.back()->Translate(glm::vec3(0, m_ball_radius - min_size/2.0f, 0));
    m_models.back()->UpdateModelMatrix();
    
    std::cout << "Generating cube map" << std::endl;
    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateSkyboxCube(m_positions, m_tex_coords, m_indices);
    m_cube_skybox.SetGeometry(m_positions, m_indices, false);
    m_cube_skybox.SetMaterial(m_cube_map_texture, m_tex_coords);
    m_cube_skybox.SetShader(m_texture_cubemap_shdader);
    m_cube_skybox.initializeForGL();

    float handle_speed{20.0f};
    float handle_mass{200.0f};
    float ball_mass{50.0f};

    m_boundary_red_player_ptr = std::make_shared<BoundaryCircle>(m_handle_radius, handle_speed, handle_mass);
    m_boundary_red_player_ptr->AssignModel(handle_red);
    m_player_red.AssignBoundary(m_boundary_red_player_ptr);

    m_boundary_green_player_ptr = std::make_shared<BoundaryCircle>(m_handle_radius, handle_speed, handle_mass);
    m_boundary_green_player_ptr->AssignModel(handle_green);
    m_player_green.AssignBoundary(m_boundary_green_player_ptr);

    m_boundary_ball_ptr = std::make_shared<BoundaryCircle>(m_ball_radius, 0.0f, ball_mass);
    m_boundary_ball_ptr->AssignModel(sphere);

    m_boundary_left_ptr = std::make_shared<BoundaryLine>(
        glm::vec2((width)/2.0f, -(lenght)/2.0f),
        glm::vec2(0.0f, 1.0f)
    );

    m_boundary_right_ptr = std::make_shared<BoundaryLine>(
        glm::vec2(-(width)/2.0f, (lenght)/2.0f),
        glm::vec2(0.0f, -1.0f)
    );

    m_boundary_red_goal_ptr = std::make_shared<BoundaryLine>(
        glm::vec2(width/2.0f, lenght/2.0f),
        glm::vec2(-1.0f, 0.0f)
    );

    m_boundary_green_goal_ptr = std::make_shared<BoundaryLine>(
        glm::vec2(-width/2.0f, -lenght/2.0f),
        glm::vec2(1.0f, 0.0f)
    );

    m_boundary_bottomleft_ptr = std::make_shared<BoundaryLine>(
        glm::vec2(goal_lenght/2.0f, -lenght/2.0f),
        glm::vec2((width-goal_lenght)/2.0f, (lenght-side_border_lenght)/2.0f)
    );

    m_boundary_bottomright_ptr = std::make_shared<BoundaryLine>(
        glm::vec2(-goal_lenght/2.0f, -lenght/2.0f),
        glm::vec2((width-goal_lenght)/2.0f, -(lenght-side_border_lenght)/2.0f)
    );

    m_boundary_topleft_ptr = std::make_shared<BoundaryLine>(
        glm::vec2(goal_lenght/2.0f, lenght/2.0f),
        glm::vec2(-(width-goal_lenght)/2.0f, (lenght-side_border_lenght)/2.0f)
    );

    m_boundary_topright_ptr = std::make_shared<BoundaryLine>(
        glm::vec2(-goal_lenght/2.0f, lenght/2.0f),
        glm::vec2(-(width-goal_lenght)/2.0f, -(lenght-side_border_lenght)/2.0f)
    );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void Match::SetUpCollisionEngine()
{
    m_collision_engine.SendBallEventsToServer = SendBallEventsToServer;
    m_collision_engine.IsBallInOnlineSide = IsBallInOnlineSide;
    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_boundary_red_player_ptr->ClearReflection();
            m_boundary_green_player_ptr->ClearReflection();
            m_boundary_ball_ptr->ClearReflection();
        }
    );
    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolvePlayerCicleXCollision(m_boundary_red_player_ptr, width/2.0f, -width/2.0f);
        }
    );
    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolvePlayerCicleZCollision(m_boundary_red_player_ptr, side_border_lenght/2.0f, 0.0f);
        }
    );

    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolvePlayerCicleXCollision(m_boundary_green_player_ptr, width/2.0f, -width/2.0f);
        }
    );
    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolvePlayerCicleZCollision(m_boundary_green_player_ptr, 0.0f, -side_border_lenght/2.0f);
        }
    );

    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolveBoundaryLineToCicleCollision(m_boundary_left_ptr, m_boundary_ball_ptr);
        }
    );
    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolveBoundaryLineToCicleCollision(m_boundary_right_ptr, m_boundary_ball_ptr);
        }
    );
    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolveBoundaryLineToCicleCollision(m_boundary_bottomleft_ptr, m_boundary_ball_ptr);
        }
    );
    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolveBoundaryLineToCicleCollision(m_boundary_bottomright_ptr, m_boundary_ball_ptr);
        }
    );
    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolveBoundaryLineToCicleCollision(m_boundary_topleft_ptr, m_boundary_ball_ptr);
        }
    );
    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolveBoundaryLineToCicleCollision(m_boundary_topright_ptr, m_boundary_ball_ptr);
        }
    );

    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolvePlayerCircleToCicleCollision(m_boundary_red_player_ptr, m_boundary_ball_ptr);
        }
    );
    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolvePlayerCircleToCicleCollision(m_boundary_green_player_ptr, m_boundary_ball_ptr);
        }
    );

    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolveGoalLineToCicleCollision(
                m_boundary_red_goal_ptr,
                m_boundary_ball_ptr,
                [this](){
                    GreenScored();
                }
            );
        }
    );
    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolveGoalLineToCicleCollision(
                m_boundary_green_goal_ptr,
                m_boundary_ball_ptr,
                [this](){
                    RedScored();
                }
            );
        }
    );
}

bool Match::IsIncomingEventNewer(const GameEventData &old_e, const GameEventData &new_e)
{
    return (new_e.m_time_stamp_now_ms > old_e.m_time_stamp_now_ms) || (new_e.m_join_level > old_e.m_join_level);
}

void Match::UpdateScoreOnline(const GameEventData &e)
{
    if (m_game_session_data.green_score != e.m_green_score)
    {
        m_ui->PlayGoalSound();
    }
    if (m_game_session_data.red_score != e.m_red_score)
    {
        m_ui->PlayGoalSound();
    }
    
    m_game_session_data.green_score = e.m_green_score;
    m_game_session_data.red_score = e.m_red_score;
}

void Match::DrawScene()
{
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_boundary_red_player_ptr->Move();
    m_boundary_green_player_ptr->Move();
    m_boundary_ball_ptr->Move();

    m_boundary_red_player_ptr->Update();
    m_boundary_green_player_ptr->Update();
    m_boundary_ball_ptr->Update();

    m_collision_engine.RunCollisionLoop(true);  //make this in another thread later


    m_pointLight.StartFillingShadowBuffer();
    for (std::shared_ptr<Model>& model : m_models)
    {
        model->DrawWithExternalShader(m_pointLight.m_shadow_map_shader);
    }
    m_pointLight.StopFillingShadowBuffer();

    m_displayBuffer.Bind();

    glDisable(GL_DEPTH_TEST);
    m_texture_cubemap_shdader.Activate();
    m_texture_cubemap_shdader.PassUniforms();
    m_cube_skybox.DrawWithExternalShader(m_texture_cubemap_shdader);
    glEnable(GL_DEPTH_TEST);

    m_blinn_phong_shdader.Activate();
    m_blinn_phong_shdader.PassUniforms();
    for (std::shared_ptr<Model>& model : m_models)
    {
        model->DrawWithInternalShader();
    }
    m_snapshotBuffer.CopyFrom(m_displayBuffer);
    m_displayBuffer.Bind();
    //blinn_phong_shdader_ptr->setInt("skybox", 2);
    //blinn_phong_shdader_ptr->setInt("scene", 3);
    m_snapshotBuffer.BindTexture(GL_TEXTURE3);
    m_floor.DrawWithInternalShader(glm::mat4(1.0f));

    m_displayBuffer.Unbind();
}

void Match::OnChangeResolution()
{
    m_camera_ptr->updatePersprectiveProj();
    m_snapshotBuffer.Resize();
    m_pointLight.Resize();

    GameScreen::OnChangeResolution();
}

void Match::OnMouseMoved(GLFWwindow *window_ptr, double xposIn, double yposIn)
{
    //m_camera_ptr->muouse_callback(window_ptr, xposIn, yposIn);
}

void Match::OnKeyPressed(GLFWwindow *window_ptr)
{
    //m_camera_ptr->processInput(window_ptr);
    std::cout << "Match::OnKeyPressed(GLFWwindow *window_ptr)" << std::endl;
}

void Match::ListenKeysPressed()
{
    if (!m_ui->m_show_settings)
    {
        m_player_red.ListenInput(PassRedInputs());
        m_player_green.ListenInput(PassGreenInputs());
    }
    online_inputs[0].x = 0.0f;
    online_inputs[0].y = 0.0f;
    ReadOnlineEvents();
    CheckAndSendLastEventsToServer();
}

void Match::ProcessPendingNavigation()
{
    if (m_ui->m_home_requested)
    {
        m_ui->m_home_requested = false;
        m_ui->Navigate_To_HomeScreen();
    }
}

void Match::Clean()
{
    SendLeaveReq();
}

void Match::SetupUI()
{
    GameScreen::SetupUI();

    SetupScoreBar();

    if(!m_match_running && !m_ui->m_show_settings)
        SetupBottomMenu();

    m_ui->DrawGlobalSettings([this](){
        UIWidgets::Checkbox("Camera Settings -------------", &m_show_camera_settings, 2.0f);
        if (m_show_camera_settings)
        {
            float height{m_camera_ptr->GHeight()};
            float distance{m_camera_ptr->GRadius()};
            float angle{m_camera_ptr->GAngle()};
            if (UIWidgets::Slider("   Height", &height, 10.0f, 200.0f, 1.5f, 200.0f))
            {
                m_camera_ptr->SetGHeight(height);
            }
            ImGui::Spacing();
            if (UIWidgets::Slider("   Distance", &distance, 10.0f, 200.0f, 1.5f, 200.0f))
            {
                m_camera_ptr->SetGRadius(distance);
            }
            ImGui::Spacing();
            if (UIWidgets::Slider("   Rotation", &angle, 0.0f, 360.0f, 1.5f, 200.0f))
            {
                m_camera_ptr->SetGAngle(angle);
            }
            ImGui::Spacing();
        }

        UIWidgets::Checkbox("Game Settings -------------", &m_show_game_settings, 2.0f);
        if (m_show_game_settings)
        {
            float ball_mass{m_boundary_ball_ptr->Mass()};
            float handle_mass{m_boundary_green_player_ptr->Mass()};
            float handle_speed{m_boundary_green_player_ptr->UserSpeedPerSecond()};
            if (UIWidgets::Slider("   Ball Mass(g)", &ball_mass, 25.0f, 100.0f, 1.5f, 200.0f))
            {
                m_boundary_ball_ptr->Mass(ball_mass);
            }
            ImGui::Spacing();
            if (UIWidgets::Slider("   Handle Mass(g)", &handle_mass, 100.0f, 300.0f, 1.5f, 200.0f))
            {
                m_boundary_red_player_ptr->Mass(handle_mass);
                m_boundary_green_player_ptr->Mass(handle_mass);
            }
            ImGui::Spacing();
            if (UIWidgets::Slider("   Handle Speed", &handle_speed, 15.0f, 50.0f, 1.5f, 200.0f))
            {
                m_boundary_red_player_ptr->SetUserSpeedPerSecond(handle_speed);
                m_boundary_green_player_ptr->SetUserSpeedPerSecond(handle_speed);
            }
            ImGui::Spacing();
        }

        UIWidgets::Checkbox("Player Red Control Settings -------------", &m_show_red_controls_settings, 2.0f);
        if (m_show_red_controls_settings)
        {
            UIWidgets::Label(m_ui->CreateDirectionString(true).c_str(), 1.5f, UIWidgets::HorizontalLayout::Left);
            bool isRedMouse = m_player_red.GetControllerType() == Controller::ControllerType::Mouse;
            if(UIWidgets::Checkbox("Enable Red Mouse Controls", &isRedMouse, 1.5f))
            {
                if(isRedMouse)
                {
                    GiveMouseControls(GameEventData::ObjectType::Red);
                    DeterminePassInputs();
                }
                else
                {
                    m_player_red.SetControllerType(old_red_keyboard);
                    DeterminePassInputs();
                }
            }
        }

        UIWidgets::Checkbox("Player Green Control Settings -------------", &m_show_green_controls_settings, 2.0f);
        if (m_show_green_controls_settings)
        {
            UIWidgets::Label(m_ui->CreateDirectionString(false).c_str(), 1.5f, UIWidgets::HorizontalLayout::Left);
            bool isGreenMouse = m_player_green.GetControllerType() == Controller::ControllerType::Mouse;
            if(UIWidgets::Checkbox("Enable Green Mouse Controls", &isGreenMouse, 1.5f))
            {
                if(isGreenMouse)
                {
                    GiveMouseControls(GameEventData::ObjectType::Green);
                    DeterminePassInputs();
                }
                else
                {
                    m_player_green.SetControllerType(old_green_keyboard);
                    DeterminePassInputs();
                }
            }
        }
    }); 
}

void Match::SetupScoreBar()
{
    m_ui->DrawScoreHUD(
        "Player Red", m_game_session_data.RedScore(),
        "Player Green", m_game_session_data.GreenScore()
    );
}

void Match::SetupBottomMenu()
{
    float display_width = m_ui->DisplaySizeX() * 0.6f;
    float startx = m_ui->DisplaySizeX() * 0.2f;
    float starty = m_ui->DisplaySizeY() * 0.3f;
    ImGui::SetNextWindowPos(ImVec2(startx, starty));
    ImGui::SetNextWindowSize(ImVec2(display_width, 0.0f));

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoSavedSettings;

    ImGui::Begin("MatchUI", nullptr, flags);

    if (!m_winner_name.empty())
    {
        const std::string text = m_winner_name + " won!";
        const float fontScale = 2.0f;
        float textY = (ImGui::GetWindowHeight() - ImGui::GetFontSize() * fontScale) * 0.5f - 40.0f;
        ImGui::SetCursorPosY(textY);
        m_ui->CenteredText(text.c_str(), fontScale);
    }

    const ImVec2 buttonSize(180.0f, 45.0f);
    ImGui::SetCursorPosY((ImGui::GetWindowHeight() - buttonSize.y) * 0.5f + 40.0f);

    if (m_ui->CenteredButton("Start Match", buttonSize.x, buttonSize.y))
    {
        InitMatch();
    }

    ImGui::End();
}

void Match::SpawnRedWithServe()
{
    m_boundary_red_player_ptr->SetVelocity(glm::vec2(0.0f));
    m_boundary_green_player_ptr->SetVelocity(glm::vec2(0.0f));
    m_boundary_ball_ptr->SetVelocity(glm::vec2(0.0f));
    int r = RandomInt(0, 1);

    m_boundary_red_player_ptr->SetPosition(m_red_spawn_points[r], player_interpolation_duration_long_ms);
    if (r == 0)
    {
        m_boundary_ball_ptr->SetPosition(m_red_spawn_points[1], ball_interpolation_duration_long_ms);
        SpawnGreenWithoutServe(1);
    }
    else
    {
        m_boundary_ball_ptr->SetPosition(m_red_spawn_points[0], ball_interpolation_duration_long_ms);
        SpawnGreenWithoutServe(0);
    }
    SendLocalPlayerDataToServer(
        m_boundary_red_player_ptr->Origin(),
        m_boundary_red_player_ptr->Velocity(),
        m_local_player_type
    );
    SendBallEventsToServer();
}

void Match::SpawnGreenWithServe()
{
    m_boundary_red_player_ptr->SetVelocity(glm::vec2(0.0f));
    m_boundary_green_player_ptr->SetVelocity(glm::vec2(0.0f));
    m_boundary_ball_ptr->SetVelocity(glm::vec2(0.0f));
    int r = RandomInt(0, 1);

    m_boundary_green_player_ptr->SetPosition(m_green_spawn_points[r], player_interpolation_duration_long_ms);
    if (r == 0)
    {
        m_boundary_ball_ptr->SetPosition(m_green_spawn_points[1], ball_interpolation_duration_long_ms);
        SpawnRedWithoutServe(1);
    }
    else
    {
        m_boundary_ball_ptr->SetPosition(m_green_spawn_points[0], ball_interpolation_duration_long_ms);
        SpawnRedWithoutServe(0);
    }
    SendLocalPlayerDataToServer(
        m_boundary_green_player_ptr->Origin(),
        m_boundary_green_player_ptr->Velocity(),
        m_local_player_type
    );
    SendBallEventsToServer();
}

void Match::SpawnRedWithoutServe(int inverse_r)
{
    if (m_match_type == MatchType::Online)
    {
        if(m_online_player_type == GameEventData::ObjectType::Red)
            return;
    }
    m_boundary_red_player_ptr->SetPosition(m_red_spawn_points[inverse_r], player_interpolation_duration_long_ms);
}

void Match::SpawnGreenWithoutServe(int inverse_r)
{
    if (m_match_type == MatchType::Online)
    {
        if(m_online_player_type == GameEventData::ObjectType::Green)
            return;
    }
    m_boundary_green_player_ptr->SetPosition(m_green_spawn_points[inverse_r], player_interpolation_duration_long_ms);
}

void Match::RedScored()
{
    m_game_session_data.AddRedScore();
    SpawnGreenWithServe();
    DetermineWinner();
}

void Match::GreenScored()
{
    m_game_session_data.AddGreenScore();
    SpawnRedWithServe();
    DetermineWinner();
}

bool Match::DetermineWinner()
{
    std::cout << std::format(
        "{} Red: {}, Green: {}\n", "Match::DetermineWinner():", m_game_session_data.red_score, m_game_session_data.green_score
    );
    if(m_game_session_data.red_score > 2)
    {
        m_winner_name = "Red";
        m_ui->PlayWinSound();
        m_match_running = false;
        return true;
    }
    if(m_game_session_data.green_score > 2)
    {
        m_winner_name = "Green";
        m_ui->PlayWinSound();
        m_match_running = false;
        return true;
    }
    return false;
}

void Match::InitMatch()
{
    m_match_running = true;
    m_winner_name = "";
    m_game_session_data.Reset();

    if (m_match_type == MatchType::Offline)
    {
        SpawnRedWithServe();
        return;
    }
    

    if(m_local_player_type == GameEventData::ObjectType::Red)
        SpawnRedWithServe();
    if (m_local_player_type == GameEventData::ObjectType::Green)
        SpawnGreenWithServe();
    
}

int Match::RandomInt(int min, int max)
{
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(m_generator);
}

void Match::GiveMouseControls(GameEventData::ObjectType player_type)
{
    switch (player_type)
    {
    case GameEventData::ObjectType::Red:
        old_red_keyboard = m_player_red.GetControllerType();
        m_player_red.SetControllerType(Controller::ControllerType::Mouse);
        if (m_player_green.GetControllerType() == Controller::ControllerType::Mouse)
        {
            m_player_green.SetControllerType(old_green_keyboard);
        }
        break;
    case GameEventData::ObjectType::Green:
        old_green_keyboard = m_player_green.GetControllerType();
        m_player_green.SetControllerType(Controller::ControllerType::Mouse);
        if (m_player_red.GetControllerType() == Controller::ControllerType::Mouse)
        {
            m_player_red.SetControllerType(old_red_keyboard);
        }
        break;
    default:
        break;
    }
}

void Match::InitializePassInputs()
{
    PassMouseXZPos = std::function<const std::vector<glm::vec2>()>{
        [this](){
            ImVec2 mouse = ImGui::GetMousePos();
            const float width  = static_cast<float>(m_shared_resolution->x);
            const float height = static_cast<float>(m_shared_resolution->y);

            // ImGui: origin is top-left.
            // OpenGL NDC: origin is center, +Y is up.
            glm::vec2 ndc{0.0f};
            ndc.x =  2.0f * mouse.x / width - 1.0f;
            ndc.y =  1.0f - 2.0f * mouse.y / height;

            glm::vec4 ray_clip{ndc.x, ndc.y, -1.0f, 1.0f};

            glm::vec4 ray_eye = m_camera_ptr->inv_proj * ray_clip;
            ray_eye = glm::vec4(ray_eye.x, ray_eye.y, ray_eye.z, 0.0f);
            glm::vec3 ray_wor = m_camera_ptr->inv_view * ray_eye;

            ray_wor = glm::normalize(ray_wor);

            float t = (-min_size/2.0f)-m_camera_ptr->position.y;
            t /= ray_wor.y;

            glm::vec2 pos {
                m_camera_ptr->position.x + t*ray_wor.x,
                m_camera_ptr->position.z + t*ray_wor.z
            };

            if (ImGui::IsKeyDown(ImGuiKey_Z))
            {
                std::cout << "t: " << t << std::endl;
                std::cout << "Cam: " << m_camera_ptr->position.x << "," << m_camera_ptr->position.y << "," << m_camera_ptr->position.z << std::endl;
                std::cout << "Dir: " << ray_wor.x << "," << ray_wor.y << "," << ray_wor.z << std::endl;
                std::cout << "Mous: " << mouse.x << "," << mouse.y << std::endl;
                std::cout << "Ndc: " << ndc.x << "," << ndc.y << std::endl;
                std::cout << "Pos: " << pos.x << "," << pos.y << std::endl;
            }
            return std::vector<glm::vec2>{pos};
        }
    };
    PassCameraControls = std::function<const std::vector<glm::vec2>()>{
        [this](){
            return m_camera_ptr->Controls_Vector;
        }
    };

    PassOnlineControls = std::function<const std::vector<glm::vec2>()>{
        [this](){
            return m_camera_ptr->Controls_Vector;
        }
    };
    SendLocalPlayerDataToServer = std::function<void(const glm::vec2&, const glm::vec2&, GameEventData::ObjectType&)>{
        [](const glm::vec2& pos, const glm::vec2& vel, GameEventData::ObjectType& player_type){
        }
    };
    SendBallEventsToServer = std::function<void()>{
        [](){
        }
    };
    SendLeaveReq = std::function<void()>{
        [](){
        }
    };
    IsBallInOnlineSide = std::function<bool(float)>{
        [](float y){
            return false;
        }
    };
    CheckAndSendLastEventsToServer = std::function<void()>{
        [](){
        }
    };

    if (m_match_type == MatchType::Online)
    {
        if (m_player_red.GetControllerType() == Controller::ControllerType::Online)
        {
            std::cout << "Match::InitializePassInputs m_boundary_online_player_ptr = m_boundary_red_player_ptr\n";
            m_boundary_online_player_ptr = m_boundary_red_player_ptr;
            m_local_player_type = GameEventData::ObjectType::Green;
            m_online_player_type = GameEventData::ObjectType::Red;
            IsBallInOnlineSide = std::function<bool(float)>{
                [](float y){
                    return y >= 0.0f;
                }
            };
        }
        else if (m_player_green.GetControllerType() == Controller::ControllerType::Online)
        {
            std::cout << "Match::InitializePassInputs m_boundary_online_player_ptr = m_boundary_green_player_ptr\n";
            m_boundary_online_player_ptr = m_boundary_green_player_ptr;
            m_local_player_type = GameEventData::ObjectType::Red;
            m_online_player_type = GameEventData::ObjectType::Green;
            IsBallInOnlineSide = std::function<bool(float)>{
                [](float y){
                    return y <= 0.0f;
                }
            };
        }

        PassOnlineControls = std::function<const std::vector<glm::vec2>()>{
            [this](){
               return online_inputs;
            }
        };
        SendLocalPlayerDataToServer = std::function<void(const glm::vec2&, const glm::vec2&, GameEventData::ObjectType&)>{
            [this](const glm::vec2& pos, const glm::vec2& vel, GameEventData::ObjectType& player_type){
                now = std::chrono::steady_clock::now();
                int time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
                GameEventData g{
                    m_game_session_data.MatchId(),
                    static_cast<int>(player_type),
                    m_game_session_data.RedScore(),
                    m_game_session_data.GreenScore(),
                    static_cast<int>(pos.x*GameEventData::factor),
                    static_cast<int>(pos.y*GameEventData::factor),
                    static_cast<int>(vel.x*GameEventData::factor),
                    static_cast<int>(vel.y*GameEventData::factor),
                    time_elapsed,
                    m_local_game_event_window.Lag()
                };
                std::shared_ptr<std::string> req{std::make_shared<std::string>(g.EncodeBuffer())};
                m_con->udpC.StartSend(req);
                m_local_game_event_window.Push(g);
                m_last_sent_local_player_event = g;
                m_is_first_local_player_event_sent = true;
                m_player_time_elapsed_till_last_send = time_elapsed;
                std::cout << std::format(
                    "{} {}\n", "Local Player Event Out:", g.EncodeBuffer()
                );
            }
        };
        SendBallEventsToServer = std::function<void()>{
            [this](){
                if(IsBallInOnlineSide(m_boundary_ball_ptr->Origin().y))
                    return;

                now = std::chrono::steady_clock::now();
                int time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
                GameEventData g{
                    m_game_session_data.MatchId(),
                    static_cast<int>(GameEventData::ObjectType::Ball),
                    m_game_session_data.RedScore(),
                    m_game_session_data.GreenScore(),
                    static_cast<int>(m_boundary_ball_ptr->Origin().x * GameEventData::factor),
                    static_cast<int>(m_boundary_ball_ptr->Origin().y * GameEventData::factor),
                    static_cast<int>(m_boundary_ball_ptr->Velocity().x * GameEventData::factor),
                    static_cast<int>(m_boundary_ball_ptr->Velocity().y * GameEventData::factor),
                    time_elapsed,
                    m_local_game_event_window.Lag()
                };
                std::shared_ptr<std::string> req{std::make_shared<std::string>(g.EncodeBuffer())};
                m_con->udpC.StartSend(req);
                m_last_sent_local_ball_event = g;
                m_is_first_local_ball_event_sent = true;
                m_ball_time_elapsed_till_last_send = time_elapsed;
                std::cout << std::format(
                    "{} {}\n", "Ball Event Out:", g.EncodeBuffer()
                );
            }
        };

        SendLeaveReq = std::function<void()>{
            [this](){
                std::shared_ptr<std::string> leave_message{std::make_shared<std::string>(
                    std::format(
                        "{}{}{}{}{}{}",
                        contract(Action::Leave),                contract(Action::Deliminator),
                        static_cast<int>(m_local_player_type),  contract(Action::Deliminator),
                        m_game_session_data.match_id,           contract(Action::EndDeliminator)
                    )
                )};
                m_con->udpC.StartSend(leave_message);
            }
        };

        CheckAndSendLastEventsToServer = std::function<void()>{
            [this](){
                now = std::chrono::steady_clock::now();
                int time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

                if(time_elapsed - m_player_time_elapsed_till_last_send > 300 && m_is_first_local_player_event_sent)
                {
                    std::shared_ptr<std::string> req{std::make_shared<std::string>(m_last_sent_local_player_event.EncodeBuffer())};
                    m_con->udpC.StartSend(req);
                    m_player_time_elapsed_till_last_send = time_elapsed;
                }
                
                if(time_elapsed - m_ball_time_elapsed_till_last_send > 300 && m_is_first_local_ball_event_sent)
                {
                    std::shared_ptr<std::string> req{std::make_shared<std::string>(m_last_sent_local_ball_event.EncodeBuffer())};
                    m_con->udpC.StartSend(req);
                    m_ball_time_elapsed_till_last_send = time_elapsed;
                }

                int time_elapsed_till_last_recieved = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_last_online_event_recieved_time).count();
                if(time_elapsed_till_last_recieved > 5000)
                    m_ui->Navigate_To_HomeScreen();
            }
        };
    }
    DeterminePassInputs();
}

void Match::DeterminePassInputs()
{
    switch (m_player_red.GetControllerType())
    {
    case Controller::ControllerType::Keyboard1:
    case Controller::ControllerType::Keyboard2:
        PassRedInputs = PassCameraControls;
        break;
    case Controller::ControllerType::Mouse:
        PassRedInputs = PassMouseXZPos;
        break;
    case Controller::ControllerType::Online:
        PassRedInputs = PassOnlineControls;
        m_player_green.SendData = SendLocalPlayerDataToServer;
        break;
    default:
        break;
    }
    switch (m_player_green.GetControllerType())
    {
    case Controller::ControllerType::Keyboard1:
    case Controller::ControllerType::Keyboard2:
        PassGreenInputs = PassCameraControls;
        break;
    case Controller::ControllerType::Mouse:
        PassGreenInputs = PassMouseXZPos;
        break;
    case Controller::ControllerType::Online:
        PassGreenInputs = PassOnlineControls;
        m_player_red.SendData = SendLocalPlayerDataToServer;
        break;
    default:
        break;
    }
}
