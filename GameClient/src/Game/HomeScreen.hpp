#pragma once

#include "GameScreen.hpp"
#include "../Renderer/Texture.hpp"
#include "../Renderer/Model.hpp"
#include "../Renderer/Shader.hpp"

class HomeScreen : public GameScreen
{
private:
    Texture m_texture;
    Shader m_screen_texture_shader;
    Model fullscreen_quad {};
    char m_player_name[64] = "";

    UIStyle m_ui_style{};
public:
    HomeScreen(
        std::shared_ptr<glm::ivec2>& shared_resolution,
        std::shared_ptr<UI>& ui_ptr,
        std::string texture_path
    );
    ~HomeScreen() override;

    void SetupUI();
    void DrawScene() override;
    void OnChangeResolution() override;
    void OnMouseMoved(GLFWwindow* window_ptr, double xposIn, double yposIn) override;
    void OnKeyPressed(GLFWwindow* window_ptr) override;
    void ListenKeysPressed() override;
    void ProcessPendingNavigation() override;
};
