#include "AssetLoader.hpp"

std::string AssetLoader::GetFullExecutablePath(std::string extension)
{
    wchar_t buffer[MAX_PATH];

    DWORD length = GetModuleFileNameW(
        nullptr,
        buffer,
        MAX_PATH
    );

    if (length == 0)
        throw std::runtime_error("Failed to get executable path");

    return std::filesystem::path(buffer).parent_path().string() + extension;
}

AssetLoader::AssetLoader(/* args */)
{
    m_asset_path = GetFullExecutablePath("/assets/");
    m_shader_path = GetFullExecutablePath("/Shaders/");

    std::cout << "m_screen_texture_shader id: " << *Shaders[ShaderId::ScreenShader].ID << std::endl;
    Shaders[ShaderId::ScreenShader].Load(m_shader_path + "screen_texture.vs.glsl", m_shader_path + "screen_texture.fs.glsl");
    Shaders[ShaderId::ScreenShader].GLCompleteShader();

    thread_ptr = std::make_unique<std::jthread>(
        [this](){
            std::cout << "Worker Spawned\n";

            std::vector<std::string> cubemap_paths{
                m_asset_path + "textures/cube-space/blue_nebula.jpeg",
                m_asset_path + "textures/cube-space/blue_nebula.jpeg",
                m_asset_path + "textures/cube-space/blue_nebula.jpeg",
                m_asset_path + "textures/cube-space/blue_nebula.jpeg",
                m_asset_path + "textures/cube-space/blue_nebula.jpeg",
                m_asset_path + "textures/cube-space/blue_nebula.jpeg"
            };
            Textures[TextureId::CubeMapTexture] = Texture{Texture::TextureType::ThreeD};
            Textures[TextureId::CubeMapTexture].Load3DTexture(cubemap_paths);

            Task CubeMapTextureLoaded{[this]() mutable {
                Textures[TextureId::CubeMapTexture].GLCompleteTexture();
                std::vector<glm::vec3> m_positions {};
                std::vector<glm::vec3> m_colors {};
                std::vector<glm::uvec3> m_indices {};
                std::vector<glm::vec2> m_tex_coords {};

                std::cout << "Generating cube map" << std::endl;
                GenerateSkyboxCube(m_positions, m_tex_coords, m_indices);
                Models[ModelId::CubeMapModel]->SetGeometry(m_positions, m_indices, false);
                Models[ModelId::CubeMapModel]->SetMaterial(Textures[TextureId::CubeMapTexture], m_tex_coords);
                Models[ModelId::CubeMapModel]->initializeForGL();
            }};
            AssetLoaderQueue.Push(
                CubeMapTextureLoaded
            );

            std::cout << "Creating shader m_texture_cubemap_shader..." << std::endl;
            Shaders[ShaderId::CubeMapShader].Load(m_shader_path + "texture_cubemap.vs.glsl", m_shader_path + "texture_cubemap.fs.glsl");
            Task CubeMapShaderLoaded{[this](){
                Shaders[ShaderId::CubeMapShader].GLCompleteShader();
            }};
            AssetLoaderQueue.Push(
                CubeMapShaderLoaded
            );

            Textures[TextureId::WallTexture] = Texture{Texture::TextureType::TwoD};
            Textures[TextureId::WallTexture].Load2DTexture(m_asset_path + "textures/tile.jpg");
            Task WallTextureLoaded{[this]() mutable {
                Textures[TextureId::WallTexture].GLCompleteTexture();

                std::vector<glm::vec3> m_positions {};
                std::vector<glm::vec3> m_colors {};
                std::vector<glm::uvec3> m_indices {};
                std::vector<glm::vec2> m_tex_coords {};

                float diagnal_length{CalculateDiagonalLength(
                    (width - goal_lenght)/2.0f,
                    (lenght - side_border_lenght)/2.0f
                )};

                std::cout << "Size Models:" << Models.size() << "\n";

                std::cout << "Creating right_border...\n";
                //right_border
                GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, min_size, min_size, side_border_lenght);
                Models[ModelId::RightBorder] = std::make_shared<Model>();
                Models[ModelId::RightBorder]->SetGeometry(m_positions, m_indices);
                Models[ModelId::RightBorder]->SetMaterial(Textures[TextureId::WallTexture], m_tex_coords);
                Models[ModelId::RightBorder]->initializeForGL();
                Models[ModelId::RightBorder]->Translate(glm::vec3(-1.0f * (width/2.0f + min_size/2.0f), 0, 0));
                Models[ModelId::RightBorder]->UpdateModelMatrix();

                std::cout << "Creating left_border...\n";
                //left_border
                m_positions.clear();
                m_colors.clear();
                m_indices.clear();
                m_tex_coords.clear();
                GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, min_size, min_size, side_border_lenght);
                Models[ModelId::LeftBorder] = std::make_shared<Model>();
                Models[ModelId::LeftBorder]->SetGeometry(m_positions, m_indices);
                Models[ModelId::LeftBorder]->SetMaterial(Textures[TextureId::WallTexture], m_tex_coords);
                Models[ModelId::LeftBorder]->initializeForGL();
                Models[ModelId::LeftBorder]->Translate(glm::vec3(1.0f * (width/2.0f + min_size/2.0f), 0, 0));
                Models[ModelId::LeftBorder]->UpdateModelMatrix();
    
                glm::vec3 translation;
                glm::vec3 rev_nor_translation;
                glm::vec3 final_translation;

                std::cout << "Creating bottom_left_border...\n";
                //bottom_left_border
                m_positions.clear();
                m_colors.clear();
                m_indices.clear();
                m_tex_coords.clear();
                GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, diagnal_length, min_size, min_size);
                Models[ModelId::BottomLeftBorder] = std::make_shared<Model>();
                Models[ModelId::BottomLeftBorder]->SetGeometry(m_positions, m_indices);
                Models[ModelId::BottomLeftBorder]->SetMaterial(Textures[TextureId::WallTexture], m_tex_coords);
                Models[ModelId::BottomLeftBorder]->initializeForGL();
                Models[ModelId::BottomLeftBorder]->RotateY(glm::radians(-39.806f));
                Models[ModelId::BottomLeftBorder]->Translate(glm::vec3(goal_lenght/2.0f, 0, -(lenght/2.0f)));
                translation = glm::vec3((width - goal_lenght)/2.0f, 0.0f, (lenght - side_border_lenght)/2.0f);
                translation = translation/2.0f;
                rev_nor_translation = glm::normalize(glm::vec3(translation.z, translation.y, -translation.x));
                final_translation = translation + (rev_nor_translation * min_size/2.0f);
                Models[ModelId::BottomLeftBorder]->Translate(final_translation);
                Models[ModelId::BottomLeftBorder]->UpdateModelMatrix();

                std::cout << "Creating bottom_right_border...\n";
                //bottom_right_border
                m_positions.clear();
                m_colors.clear();
                m_indices.clear();
                m_tex_coords.clear();
                GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, diagnal_length, min_size, min_size);
                Models[ModelId::BottomRightBorder] = std::make_shared<Model>();
                Models[ModelId::BottomRightBorder]->SetGeometry(m_positions, m_indices);
                Models[ModelId::BottomRightBorder]->SetMaterial(Textures[TextureId::WallTexture], m_tex_coords);
                Models[ModelId::BottomRightBorder]->initializeForGL();
                Models[ModelId::BottomRightBorder]->RotateY(glm::radians(39.806f));
                Models[ModelId::BottomRightBorder]->Translate(glm::vec3(-goal_lenght/2.0f, 0, -(lenght/2.0f)));
                translation = glm::vec3(-(width - goal_lenght)/2.0f, 0.0f, (lenght - side_border_lenght)/2.0f);
                translation = translation/2.0f;
                rev_nor_translation = glm::normalize(glm::vec3(translation.z, translation.y, -translation.x));
                final_translation = translation - (rev_nor_translation * min_size/2.0f);
                Models[ModelId::BottomRightBorder]->Translate(final_translation);
                Models[ModelId::BottomRightBorder]->UpdateModelMatrix();
            
                std::cout << "Creating top_left_border...\n";
                //top_left_border
                m_positions.clear();
                m_colors.clear();
                m_indices.clear();
                m_tex_coords.clear();
                GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, diagnal_length, min_size, min_size);
                Models[ModelId::TopLeftBorder] = std::make_shared<Model>();
                Models[ModelId::TopLeftBorder]->SetGeometry(m_positions, m_indices);
                Models[ModelId::TopLeftBorder]->SetMaterial(Textures[TextureId::WallTexture], m_tex_coords);
                Models[ModelId::TopLeftBorder]->initializeForGL();
                Models[ModelId::TopLeftBorder]->RotateY(glm::radians(39.806f));
                Models[ModelId::TopLeftBorder]->Translate(glm::vec3(goal_lenght/2.0f, 0, (lenght/2.0f)));
                translation = glm::vec3((width - goal_lenght)/2.0f, 0.0f, -(lenght - side_border_lenght)/2.0f);
                translation = translation/2.0f;
                rev_nor_translation = glm::normalize(glm::vec3(translation.z, translation.y, -translation.x));
                final_translation = translation - (rev_nor_translation * min_size/2.0f);
                Models[ModelId::TopLeftBorder]->Translate(final_translation);
                Models[ModelId::TopLeftBorder]->UpdateModelMatrix();

                std::cout << "Creating top_right_border...\n";
                //top_right_border
                m_positions.clear();
                m_colors.clear();
                m_indices.clear();
                m_tex_coords.clear();
                GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, diagnal_length, min_size, min_size);
                Models[ModelId::TopRightBorder] = std::make_shared<Model>();
                Models[ModelId::TopRightBorder]->SetGeometry(m_positions, m_indices);
                Models[ModelId::TopRightBorder]->SetMaterial(Textures[TextureId::WallTexture], m_tex_coords);
                Models[ModelId::TopRightBorder]->initializeForGL();
                Models[ModelId::TopRightBorder]->RotateY(glm::radians(-39.806f));
                Models[ModelId::TopRightBorder]->Translate(glm::vec3(-goal_lenght/2.0f, 0, (lenght/2.0f)));
                translation = glm::vec3(-(width - goal_lenght)/2.0f, 0.0f, -(lenght - side_border_lenght)/2.0f);
                translation = translation/2.0f;
                rev_nor_translation = glm::normalize(glm::vec3(translation.z, translation.y, -translation.x));
                final_translation = translation + (rev_nor_translation * min_size/2.0f);
                Models[ModelId::TopRightBorder]->Translate(final_translation);
                Models[ModelId::TopRightBorder]->UpdateModelMatrix();
            }};
            AssetLoaderQueue.Push(
                WallTextureLoaded
            );

            Textures[TextureId::FloorTexture] = Texture{Texture::TextureType::TwoD};
            Textures[TextureId::FloorTexture].Load2DTexture(m_asset_path + "textures/base.png");
            Task FloorTextureLoaded{[this]() mutable {
                Textures[TextureId::FloorTexture].GLCompleteTexture();

                std::vector<glm::vec3> m_positions {};
                std::vector<glm::vec3> m_colors {};
                std::vector<glm::uvec3> m_indices {};
                std::vector<glm::vec2> m_tex_coords {};

                std::cout << "Generating cube map" << std::endl;
                GenerateXZBase(m_positions, m_colors, m_indices, m_tex_coords, width, lenght, goal_lenght, side_border_lenght);
                Models[ModelId::FloorModel] = std::make_shared<Model>();
                Models[ModelId::FloorModel]->SetGeometry(m_positions, m_indices);
                Models[ModelId::FloorModel]->SetMaterial(Textures[TextureId::FloorTexture], m_tex_coords);
                Models[ModelId::FloorModel]->m_enable_reflection = true;
                Models[ModelId::FloorModel]->initializeForGL();
                Models[ModelId::FloorModel]->Translate(glm::vec3(0, -min_size/2.0f, 0));
                Models[ModelId::FloorModel]->RotateY(0.0f);
                Models[ModelId::FloorModel]->UpdateModelMatrix();
                //m_models.emplace_back(m_floor);
            }};
            AssetLoaderQueue.Push(
                FloorTextureLoaded
            );

            Textures[TextureId::SphereTexture] = Texture{Texture::TextureType::TwoD};
            Textures[TextureId::SphereTexture].Load2DTexture(m_asset_path + "textures/ball.png");
            Task SphereTextureLoaded{[this]() mutable {
                Textures[TextureId::SphereTexture].GLCompleteTexture();

                std::vector<glm::vec3> m_positions {};
                std::vector<glm::vec3> m_colors {};
                std::vector<glm::uvec3> m_indices {};
                std::vector<glm::vec2> m_tex_coords {};

                std::cout << "Generating cube map" << std::endl;
                GenerateSphere(m_positions, m_colors, m_indices, m_tex_coords, 16, m_ball_radius);
                Models[ModelId::SphereModel] = std::make_shared<Model>();
                Models[ModelId::SphereModel]->SetGeometry(m_positions, m_indices, true);
                Models[ModelId::SphereModel]->SetMaterial(Textures[TextureId::SphereTexture], m_tex_coords);
                Models[ModelId::SphereModel]->initializeForGL();
                Models[ModelId::SphereModel]->Translate(glm::vec3(0, m_ball_radius - min_size/2.0f, 0));
                Models[ModelId::SphereModel]->UpdateModelMatrix();
                //m_models.emplace_back(m_floor);
            }};
            AssetLoaderQueue.Push(
                SphereTextureLoaded
            );

            std::cout << "Shaders[ShaderId::BlinnPhongShader] id: " << *Shaders[ShaderId::BlinnPhongShader].ID << std::endl;
            Shaders[ShaderId::BlinnPhongShader].Load(m_shader_path + "blinn_phong.vs.glsl", m_shader_path + "blinn_phong.fs.glsl");
            Task BlinnPhongShaderLoaded{[this](){
                Shaders[ShaderId::BlinnPhongShader].GLCompleteShader();
                Shaders[ShaderId::BlinnPhongShader].Activate();
                Shaders[ShaderId::BlinnPhongShader].setInt("ourTexture", 0);
                Shaders[ShaderId::BlinnPhongShader].setInt("shTex", 1);
                Shaders[ShaderId::BlinnPhongShader].setInt("skybox", 2);
                Shaders[ShaderId::BlinnPhongShader].setInt("scene", 3);
            }};
            AssetLoaderQueue.Push(
                BlinnPhongShaderLoaded
            );

            std::cout << "m_shadow_map_shdader id: " << *Shaders[ShaderId::ShadowMapShader].ID << std::endl;
            Shaders[ShaderId::ShadowMapShader].Load(m_shader_path + "shadow_map.vs.glsl", m_shader_path + "shadow_map.fs.glsl");
            std::cout << "Creating shader m_texture_cubemap_shdader..." << std::endl;
            Task ShadowMapShaderLoaded{[this](){
                Shaders[ShaderId::ShadowMapShader].GLCompleteShader();
            }};
            AssetLoaderQueue.Push(
                ShadowMapShaderLoaded
            );

            Task SetAllAssetsLoaded{[this](){
                m_all_assets_loaded = true;
                thread_ptr->join();
            }};
            AssetLoaderQueue.Push(
                SetAllAssetsLoaded
            );
        }
    );
}

AssetLoader::~AssetLoader()
{
}

void AssetLoader::CompleteLoad()
{
    if (m_all_assets_loaded)
        return;
    
    Task t;
    while (AssetLoaderQueue.Read(t))
    {
        std::cout << "Task Read\n";
        t();
    }
}
