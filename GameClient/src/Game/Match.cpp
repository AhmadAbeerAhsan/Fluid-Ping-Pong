#include "Match.hpp"

Match::Match(
    Controller::ControllerType player1_type, Controller::ControllerType player2_type,
    std::shared_ptr<glm::ivec2>& shared_resolution,
    std::shared_ptr<UI>& ui_ptr
) : GameScreen(shared_resolution, ui_ptr),
    m_player1(player1_type), m_player2(player2_type),
    m_snapshotBuffer(Framebuffer::FrameBufferType::Color_FloatAlpha, shared_resolution),
    m_pointLight(m_shared_resolution),
    m_collision_engine{}
{
    InitScene();
    SetUpCollisionEngine();
}

Match::~Match()
{
    std::cout << "Match::~Match()" << std::endl;
}

void Match::InitScene()
{
    m_camera_ptr = std::make_shared<Camera>(
        glm::vec3(-100.0f, 70.0f, 0.0f), 
        glm::vec3(0.0f, 0.0f, 1.0f), 
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        m_shared_resolution,
        45.0f,
        0.01f, 1000.0f
    );

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
    std::shared_ptr<Model> handle{std::make_shared<Model>()};
    m_models.emplace_back(handle);
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
    std::shared_ptr<Model> handle2{std::make_shared<Model>()};
    m_models.emplace_back(handle2);
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

    m_boundary_p1_ptr = std::make_shared<BoundaryCircle>(m_handle_radius, handle_speed, handle_mass);
    m_boundary_p1_ptr->AssignModel(handle);
    m_player1.AssignBoundary(m_boundary_p1_ptr);

    m_boundary_p2_ptr = std::make_shared<BoundaryCircle>(m_handle_radius, handle_speed, handle_mass);
    m_boundary_p2_ptr->AssignModel(handle2);

    m_boundary_ball_ptr = std::make_shared<BoundaryCircle>(m_ball_radius, 0.0f, ball_mass);
    m_boundary_ball_ptr->AssignModel(sphere);
    m_player2.AssignBoundary(m_boundary_p2_ptr);

    m_boundary_left_ptr = std::make_shared<BoundaryLine>(
        glm::vec2((width)/2.0f, -(lenght)/2.0f),
        glm::vec2(0.0f, 1.0f)
    );

    m_boundary_right_ptr = std::make_shared<BoundaryLine>(
        glm::vec2(-(width)/2.0f, (lenght)/2.0f),
        glm::vec2(0.0f, -1.0f)
    );

    m_boundary_topgoal_ptr = std::make_shared<BoundaryLine>(
        glm::vec2(width/2.0f, lenght/2.0f),
        glm::vec2(-1.0f, 0.0f)
    );

    m_boundary_bottomgoal_ptr = std::make_shared<BoundaryLine>(
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
    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_boundary_p1_ptr->ClearReflection();
            m_boundary_p2_ptr->ClearReflection();
            m_boundary_ball_ptr->ClearReflection();
        }
    );
    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolvePlayerCicleXCollision(m_boundary_p1_ptr, width/2.0f, -width/2.0f);
        }
    );
    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolvePlayerCicleZCollision(m_boundary_p1_ptr, side_border_lenght/2.0f, 0.0f);
        }
    );

    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolvePlayerCicleXCollision(m_boundary_p2_ptr, width/2.0f, -width/2.0f);
        }
    );
    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolvePlayerCicleZCollision(m_boundary_p2_ptr, 0.0f, -side_border_lenght/2.0f);
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
            m_collision_engine.ResolveBoundaryLineToCicleCollision(m_boundary_topgoal_ptr, m_boundary_ball_ptr);
        }
    );
    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolveBoundaryLineToCicleCollision(m_boundary_bottomgoal_ptr, m_boundary_ball_ptr);
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
            m_collision_engine.ResolvePlayerCircleToCicleCollision(m_boundary_p1_ptr, m_boundary_ball_ptr);
        }
    );
    m_collision_engine.CollisionLoop.emplace_back(
        [this](){
            m_collision_engine.ResolvePlayerCircleToCicleCollision(m_boundary_p2_ptr, m_boundary_ball_ptr);
        }
    );
}

void Match::SetupUI()
{
    GameScreen::SetupUI();

    

}

void Match::DrawScene()
{
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_boundary_p1_ptr->Move();
    m_boundary_p2_ptr->Move();
    m_boundary_ball_ptr->Move();

    m_boundary_p1_ptr->Update();
    m_boundary_p2_ptr->Update();
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
    m_camera_ptr->muouse_callback(window_ptr, xposIn, yposIn);
}

void Match::OnKeyPressed(GLFWwindow *window_ptr)
{
    m_camera_ptr->processInput(window_ptr);
    std::cout << "Match::OnKeyPressed(GLFWwindow *window_ptr)" << std::endl;
}

void Match::ListenKeysPressed()
{
    m_player1.ListenInput();
    m_player2.ListenInput();
}
