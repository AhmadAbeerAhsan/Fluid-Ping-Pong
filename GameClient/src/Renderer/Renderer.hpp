#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"

struct Timer
{
    float currentTime = 0.0f;
    float deltaTime = 0.0f;
    float lastTime = 0.0f;

    void calculateDelta()
    {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
    }
};

class Renderer
{
    public:
        Renderer(int _screen_width, int _screen_height);
        ~Renderer();

        bool shouldClose() { return glfwWindowShouldClose(window); }

        void clear();
        void render();

        const glm::vec3& GetCameraPosition();

        Timer timer{};
        Camera camera;

    private:
        int screen_width; int screen_height;
        GLFWwindow* window;

        void intializeGLFW(int width, int height);
        void processInput();

        static void framebuffer_size_callback(GLFWwindow* window, int _screen_width, int _screen_height);
        void onFramebufferResize(int _screen_width, int _screen_height);

        static void muouse_callback_static(GLFWwindow *window, double xposIn, double yposIn);
        void muouse_callback(double xposIn, double yposIn);
};