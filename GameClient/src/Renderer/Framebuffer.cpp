#include "Framebuffer.hpp"

Framebuffer::Framebuffer(FrameBufferType framebufferType, std::shared_ptr<glm::ivec2> shared_resolution) :
    m_fbo_id{new GLuint(0)},
    m_texture_id{new GLuint(0)},
    m_rbo_id{new GLuint(0)},
    m_shared_resolution(shared_resolution),
    m_type{framebufferType}
{
    glGenFramebuffers(1, m_fbo_id.get());
    glBindFramebuffer(GL_FRAMEBUFFER, *m_fbo_id);

    glGenTextures(1, m_texture_id.get());
    glBindTexture(GL_TEXTURE_2D, *m_texture_id);

    switch (framebufferType)
    {
    case FrameBufferType::Color:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_shared_resolution->x, m_shared_resolution->y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *m_texture_id, 0);

        glGenRenderbuffers(1, m_rbo_id.get());
        glBindRenderbuffer(GL_RENDERBUFFER, *m_rbo_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_shared_resolution->x, m_shared_resolution->y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *m_rbo_id);

        m_copyMask = GL_COLOR_BUFFER_BIT |
                   GL_DEPTH_BUFFER_BIT |
                   GL_STENCIL_BUFFER_BIT;
        break;
    case FrameBufferType::Color_FloatAlpha:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_shared_resolution->x, m_shared_resolution->y, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *m_texture_id, 0);

        glGenRenderbuffers(1, m_rbo_id.get());
        glBindRenderbuffer(GL_RENDERBUFFER, *m_rbo_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_shared_resolution->x, m_shared_resolution->y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *m_rbo_id);

        m_copyMask = GL_COLOR_BUFFER_BIT |
                   GL_DEPTH_BUFFER_BIT |
                   GL_STENCIL_BUFFER_BIT;
        break;
    case FrameBufferType::Depth_Stencil:
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_shared_resolution->x, m_shared_resolution->y, 0, 
            GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, *m_texture_id, 0);

        m_copyMask = GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
        break;
    case FrameBufferType::Shadow_Map:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
            m_shared_resolution->x, m_shared_resolution->y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

        // may reduce shadow border artifacts
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *m_texture_id, 0); // <-- was missing

        m_copyMask =
                   GL_DEPTH_BUFFER_BIT;

        break;
    default:
        break;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, *m_fbo_id);
    glViewport(0, 0, m_shared_resolution->x, m_shared_resolution->y); 
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::BindTexture(int gl_texPos)
{
    glActiveTexture(gl_texPos);
    glBindTexture(GL_TEXTURE_2D, *m_texture_id);
}

void Framebuffer::UnBindTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::CopyFrom(const Framebuffer &source)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, *source.m_fbo_id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, *m_fbo_id);
    glBlitFramebuffer(
        0, 0, m_shared_resolution->x, m_shared_resolution->y,
        0, 0, m_shared_resolution->x, m_shared_resolution->y,
        m_copyMask,
        GL_NEAREST
    );
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::ClearBufferForNextDraw()
{
    //glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, *m_fbo_id);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize()
{
    std::cout << "Framebuffer::Resize() called, fbo id: " << *m_fbo_id << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, *m_fbo_id);
    glBindTexture(GL_TEXTURE_2D, *m_texture_id);

    switch (m_type)
    {
    case FrameBufferType::Color:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_shared_resolution->x, m_shared_resolution->y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindRenderbuffer(GL_RENDERBUFFER, *m_rbo_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_shared_resolution->x, m_shared_resolution->y);
        break;
    case FrameBufferType::Color_FloatAlpha:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_shared_resolution->x, m_shared_resolution->y, 0, GL_RGBA, GL_FLOAT, NULL);
        glBindRenderbuffer(GL_RENDERBUFFER, *m_rbo_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_shared_resolution->x, m_shared_resolution->y);
        break;
    case FrameBufferType::Depth_Stencil:
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_shared_resolution->x, m_shared_resolution->y, 0,
            GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
        );
        break;
    case FrameBufferType::Shadow_Map:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
            m_shared_resolution->x, m_shared_resolution->y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        break;
    default:
        break;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete after resize!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}