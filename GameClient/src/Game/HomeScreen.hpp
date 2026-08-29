#pragma once

#include "GameScreen.hpp"
#include "Controller.hpp"
#include "../Renderer/Texture.hpp"
#include "../Renderer/Model.hpp"
#include "../Renderer/Shader.hpp"

class HomeScreen : public GameScreen
{
private:
    Texture m_texture;
    Shader m_screen_texture_shader;
    Model fullscreen_quad {};
    
    char m_player_name[32] = "";
    char m_match_name[32] = "";
    bool m_show_matchmaking_menu{false};
    int p1, p2, c1, c2;

public:
    HomeScreen(
        std::shared_ptr<glm::ivec2>& shared_resolution,
        std::shared_ptr<UI>& ui_ptr,
        std::shared_ptr<Connection>& con,
        std::string texture_path
    );
    ~HomeScreen() override;

    void SetupUI();
    void DrawMenu();
    void DrawOnlineMatchMakingMenu();
    void DrawScene() override;
    void OnChangeResolution() override;
    void OnMouseMoved(GLFWwindow* window_ptr, double xposIn, double yposIn) override;
    void OnKeyPressed(GLFWwindow* window_ptr) override;
    void ListenKeysPressed() override;
    void ProcessPendingNavigation() override;
};
