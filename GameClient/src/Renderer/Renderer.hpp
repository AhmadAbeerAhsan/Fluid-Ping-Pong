#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"

#include <iostream>

class Renderer
{
    public:
        Renderer(std::shared_ptr<glm::ivec2> shared_resolution, std::shared_ptr<Camera> camera);
        ~Renderer();

        void clear();
        void render();

        std::shared_ptr<Camera> m_camera;

    private:
        std::shared_ptr<glm::ivec2> m_shared_resolution;
};