#pragma once

#include <memory>
#include <iostream>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    /* data */
public:
    Framebuffer(FrameBufferType framebufferType, std::shared_ptr<glm::ivec2> shared_resolution);

    void Bind();
    void Unbind();

    void BindTexture(int gl_texPos);
    void UnBindTexture();
    void CopyFrom(const Framebuffer &source);
    void ClearBufferForNextDraw();
    void Resize();

    std::unique_ptr<GLuint> m_fbo_id;
    std::unique_ptr<GLuint> m_texture_id;
    std::unique_ptr<GLuint> m_rbo_id;

    std::shared_ptr<glm::ivec2> m_shared_resolution;
    FrameBufferType m_type;
    GLbitfield m_copyMask;
    
};