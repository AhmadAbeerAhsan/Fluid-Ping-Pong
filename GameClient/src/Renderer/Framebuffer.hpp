#pragma once

#include <memory>
#include <iostream>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct FramebufferDeleter
{
    void operator()(GLuint* id) const noexcept
    {
        if (id)
        {
            if (*id != 0)
            {
                std::cout << "glDeleteFramebuffers(1, id): " << *id << std::endl;
                glDeleteFramebuffers(1, id);
            }
            delete id;
        }
    }
};
 
struct TextureDeleter
{
    void operator()(GLuint* id) const noexcept
    {
        if (id)
        {
            if (*id != 0)
            {
                std::cout << "glDeleteTextures(1, id): " << *id << std::endl;
                glDeleteTextures(1, id);
            }
            delete id;
        }
    }
};
 
struct RenderbufferDeleter
{
    void operator()(GLuint* id) const noexcept
    {
        if (id)
        {
            if (*id != 0)
            {
                std::cout << "glDeleteRenderbuffers(1, id): " << *id << std::endl;
                glDeleteRenderbuffers(1, id);
            }
            delete id;
        }
    }
};
 
using FramebufferUniquePtr  = std::unique_ptr<GLuint, FramebufferDeleter>;
using TextureUniquePtr      = std::unique_ptr<GLuint, TextureDeleter>;
using RenderbufferUniquePtr = std::unique_ptr<GLuint, RenderbufferDeleter>;

class Framebuffer
{
public:
    enum FrameBufferType
    {
        Color,
        Color_FloatAlpha,
        Shadow_Map,
        Depth,
        Stencil,
        Depth_Stencil    
    };

private:
    FramebufferUniquePtr m_fbo_id;
    TextureUniquePtr m_texture_id;
    RenderbufferUniquePtr m_rbo_id;

    std::shared_ptr<glm::ivec2> m_shared_resolution;
    FrameBufferType m_type;
    GLbitfield m_copyMask;
public:
    Framebuffer(FrameBufferType framebufferType, std::shared_ptr<glm::ivec2> shared_resolution);

    void Bind();
    void Unbind();

    void BindTexture(int gl_texPos);
    void UnBindTexture();
    void CopyFrom(const Framebuffer &source);
    void ClearBufferForNextDraw();
    void Resize();
};