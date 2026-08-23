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
    std::shared_ptr<GLuint> m_texture_id;

    enum TextureType
    {
        None,
        TwoD,
        ThreeD
    };
    TextureType m_texture_type{TextureType::None};

public:
    Texture();
    Texture(const char* path);
    Texture(std::vector<std::string> paths);

    void Use(int gl_texPos) const;
};