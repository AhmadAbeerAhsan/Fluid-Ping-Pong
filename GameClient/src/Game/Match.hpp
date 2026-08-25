#pragma once

#include "Controller.hpp"
#include "GameScreen.hpp"
#include "BoundaryCircle.hpp"
#include "BoundaryLine.hpp"
#include "CollisionEngine.hpp"
#include "../Renderer/Renderer.hpp"
#include "../Renderer/PointLight.hpp"

class Match : public GameScreen
{
private:
    Controller m_player1, m_player2;
    std::shared_ptr<Boundary> 
        m_boundary_p1_ptr, m_boundary_p2_ptr,
        m_boundary_ball_ptr,
        m_boundary_left_ptr, m_boundary_right_ptr,
        m_boundary_topgoal_ptr, m_boundary_bottomgoal_ptr,
        m_boundary_bottomleft_ptr, m_boundary_bottomright_ptr,
        m_boundary_topleft_ptr, m_boundary_topright_ptr;

    //Game Objects
    std::shared_ptr<Camera> m_camera_ptr;

    Framebuffer m_snapshotBuffer;

    PointLight m_pointLight;

    Texture m_cube_map_texture;

    Shader m_blinn_phong_shdader;
    Shader m_shadow_map_shdader;
    Shader m_screen_texture_shader;
    Shader m_texture_cubemap_shdader;

    std::vector<std::shared_ptr<Model>> m_models{};
    Model m_cube_skybox{};
    Model m_floor{};

    float m_ball_radius{2.0f};
    float m_handle_radius{};

    CollisionEngine m_collision_engine;

    void InitScene();
    void SetUpCollisionEngine();

public:
    Match(
        Controller::ControllerType player1_type, Controller::ControllerType player2_type,
        std::shared_ptr<glm::ivec2>& shared_resolution,
        std::shared_ptr<UI>& ui_ptr
    );
    ~Match() override;

    void SetupUI() override;
    void DrawScene() override;
    void OnChangeResolution() override;
    void OnMouseMoved(GLFWwindow* window_ptr, double xposIn, double yposIn) override;
    void OnKeyPressed(GLFWwindow* window_ptr) override;
    void ListenKeysPressed() override;

private:
    float lenght{100.0f};
    float width{50.0f};
    float goal_lenght{20.0f};
    float side_border_lenght{75.0f};
    float min_size{3.0f};
};
