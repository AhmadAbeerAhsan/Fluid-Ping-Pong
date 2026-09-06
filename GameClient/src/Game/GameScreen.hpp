#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "../Renderer/Framebuffer.hpp"
#include "GameAssetsGenerator.hpp"
#include "../UI/UI.hpp"
#include "../Connection/Connection.hpp"
#include "../Connection/ClientServerContract.hpp"

#include <format>

class GameScreen
{
protected:
    std::shared_ptr<glm::ivec2> m_shared_resolution;
    Framebuffer m_displayBuffer;
    Framebuffer m_resolveBuffer; // for ms
    std::shared_ptr<UI> m_ui;
    std::shared_ptr<Connection> m_con;
public:
    GameScreen(
        std::shared_ptr<glm::ivec2>& shared_resolution,
        std::shared_ptr<UI>& ui_ptr,
        std::shared_ptr<Connection>& con,
        int samples = 0
    );

    virtual ~GameScreen();

    virtual void SetupUI();
    virtual void DrawScene() = 0;
    virtual void UseScenceAsTexture();
    virtual void ClearScene();

    virtual void OnChangeResolution();
    virtual void OnMouseMoved(GLFWwindow* window_ptr, double xposIn, double yposIn) = 0;
    virtual void OnKeyPressed(GLFWwindow* window_ptr) = 0;
    virtual void ListenKeysPressed() = 0;
    virtual void ProcessPendingNavigation() = 0;
    virtual void Clean() = 0;
};