#pragma once

#include <memory>
#include <thread>

#include "Texture.hpp"
#include "Model.hpp"
#include "../Game/GameAssetsGenerator.hpp"
#include "../Utilities/LockFreeQueue.hpp"
#include <filesystem>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using Task = std::function<void()>;

class AssetLoader
{
private:
    LockFreeQueue<Task> AssetLoaderQueue{};
    std::unique_ptr<std::jthread> thread_ptr;
    std::string m_asset_path;
    std::string m_shader_path;

    bool m_all_assets_loaded{false};
    std::string GetFullExecutablePath(std::string extension);
public:
    AssetLoader(/* args */);
    ~AssetLoader();

    void CompleteLoad();

    enum ModelId : int
    {
        CubeMapModel,
        RightBorder,
        LeftBorder,
        BottomLeftBorder,
        BottomRightBorder,
        TopLeftBorder,
        TopRightBorder,
        SphereModel,
        FloorModel,
        CountModel
    };

    enum ShaderId : int
    {
        CubeMapShader,
        BlinnPhongShader,
        ShadowMapShader,
        ScreenShader,
        CountShader
    };

    enum TextureId : int
    {
        CubeMapTexture,
        WallTexture,
        FloorTexture,
        SphereTexture,
        CountTexture
    };

    std::array<std::shared_ptr<Model>, ModelId::CountModel> Models{std::make_shared<Model>()};
    std::array<Shader, ShaderId::CountShader> Shaders{};
    std::array<Texture, TextureId::CountTexture> Textures{};

    float lenght{100.0f};
    float width{50.0f};
    float goal_lenght{20.0f};
    float side_border_lenght{75.0f};
    float min_size{3.0f};
    float m_ball_radius{2.0f};
};