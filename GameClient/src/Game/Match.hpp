#pragma once

#include "Controller.hpp"
#include "GameScreen.hpp"
#include "../Renderer/Renderer.hpp"
#include "../Renderer/PointLight.hpp"

class Match : public GameScreen
{
private:
    Controller m_player1, m_player2;

    //Game Objects
    std::shared_ptr<Camera> m_camera_ptr;

    Framebuffer m_snapshotBuffer;

    PointLight m_pointLight;

    Texture m_cube_map_texture;

    Shader m_blinn_phong_shdader;
    Shader m_shadow_map_shdader;
    Shader m_screen_texture_shader;
    Shader m_texture_cubemap_shdader;

    std::vector<Model> m_models{};
    Model m_cube_skybox{};
    Model m_floor{};

    void InitScene();
public:
    Match(
        Controller::ControllerType player1_type, Controller::ControllerType player2_type,
        std::shared_ptr<glm::ivec2> shared_resolution
    );
    ~Match() override;

    void DrawScene() override;
    void OnChangeResolution() override;
    void OnMouseMoved(GLFWwindow* window_ptr, double xposIn, double yposIn) override;
    void OnKeyPressed(GLFWwindow* window_ptr) override;
};
