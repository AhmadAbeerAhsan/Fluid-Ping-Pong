#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "../Renderer/Framebuffer.hpp"
#include "GameAssetsGenerator.hpp"

class GameScreen
{
protected:
    std::shared_ptr<glm::ivec2> m_shared_resolution;
    Framebuffer m_displayBuffer;

public:
    GameScreen(
        std::shared_ptr<glm::ivec2> shared_resolution
    );

    virtual ~GameScreen();

    virtual void DrawScene() = 0;
    virtual void UseScenceAsTexture();
    virtual void ClearScene();

    virtual void OnChangeResolution();
    virtual void OnMouseMoved(GLFWwindow* window_ptr, double xposIn, double yposIn) = 0;
    virtual void OnKeyPressed(GLFWwindow* window_ptr) = 0;
};