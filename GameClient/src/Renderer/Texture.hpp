#pragma once

#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <array>
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture
{
public:
    enum TextureType
    {
        None,
        TwoD,
        ThreeD
    };
private:
    std::shared_ptr<GLuint> m_texture_id;
    TextureType m_texture_type{TextureType::None};

    unsigned char* m_2d_raw_data = nullptr;
    std::array<unsigned char*, 6> m_3d_raw_datas{nullptr};

    GLenum m_color_code;
    int m_width, m_height;
public:
    Texture(TextureType type);
    Texture(){};

    void Load2DTexture(const char* path);
    void Load3DTexture(std::vector<std::string> paths);

    void GLCompleteTexture();

    void Use(int gl_texPos) const;
};