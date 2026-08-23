#pragma once

#include <memory>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class AppWindow
{
private:
    std::shared_ptr<glm::ivec2> m_shared_resolution;
    std::unique_ptr<GLFWwindow*> m_window_ptr;

    static void framebuffer_size_callback(GLFWwindow* window, int _screen_width, int _screen_height);
    void onFramebufferResize(int _screen_width, int _screen_height);

    static void muouse_callback_static(GLFWwindow *window, double xposIn, double yposIn);
    void muouse_callback(double xposIn, double yposIn);
public:
    AppWindow(std::shared_ptr<glm::ivec2> shared_resolution);
    ~AppWindow();

    void RecordEvents();
    void ProcessEvents();
    void Display();

    bool shouldClose() { return glfwWindowShouldClose(*m_window_ptr); }

    std::function<void()> UpdateResolutionSubscibedCallback;
    std::function<void(GLFWwindow*, double, double)> MouseMovedCallback;
    std::function<void(GLFWwindow*)> KeyPressedCallback;
};