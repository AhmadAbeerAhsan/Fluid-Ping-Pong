#pragma once

#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <functional>

#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture
{
private:
    std::unique_ptr<GLuint> m_texture_id;

public:
    Texture(const char* path);
    Texture(std::vector<std::string> paths);

    std::function<void(int)> Use;
};

