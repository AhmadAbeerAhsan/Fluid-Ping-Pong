#pragma once

#include <memory>
#include <iostream>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Framebuffer
{
public:
    enum FrameBufferType
    {
        Color,
        Depth,
        Stencil,
        Depth_Stencil    
    };

private:
    /* data */
public:
    Framebuffer(FrameBufferType framebufferType, int width, int heigth);

    std::function<void()> Bind;
    void Unbind();

    void BindTexture(int gl_texPos);
    void UnBindTexture();

    std::unique_ptr<GLuint> m_fbo_id;
    std::unique_ptr<GLuint> m_texture_id;
    std::unique_ptr<GLuint> m_rbo_id;

};