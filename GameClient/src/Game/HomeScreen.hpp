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
public:
    HomeScreen(std::shared_ptr<glm::ivec2> shared_resolution, std::string texture_path);
    ~HomeScreen() override;

    virtual void DrawScene();
    virtual void OnChangeResolution();
    virtual void OnMouseMoved(GLFWwindow* window_ptr, double xposIn, double yposIn);
    virtual void OnKeyPressed(GLFWwindow* window_ptr);
};
