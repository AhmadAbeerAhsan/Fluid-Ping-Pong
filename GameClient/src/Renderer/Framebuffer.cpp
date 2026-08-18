#include "Framebuffer.hpp"

Framebuffer::Framebuffer(FrameBufferType framebufferType, int width, int heigth) :
    m_fbo_id{std::make_unique<GLuint>()},
    m_texture_id{std::make_unique<GLuint>()},
    m_rbo_id{std::make_unique<GLuint>()}
{
    glGenFramebuffers(1, m_fbo_id.get());
    glBindFramebuffer(GL_FRAMEBUFFER, *m_fbo_id);

    glGenTextures(1, m_texture_id.get());
    glBindTexture(GL_TEXTURE_2D, *m_texture_id);
    
    switch (framebufferType)
    {
    case FrameBufferType::Color:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *m_texture_id, 0);

        glGenRenderbuffers(1, m_rbo_id.get());
        glBindRenderbuffer(GL_RENDERBUFFER, *m_rbo_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, heigth);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *m_rbo_id);

        Bind = std::function<void()>{
            [this](){
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glBindFramebuffer(GL_FRAMEBUFFER, *m_fbo_id);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            }
        };
        break;
    case FrameBufferType::Depth_Stencil:
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, heigth, 0, 
            GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, *m_texture_id, 0);
    default:
        break;
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
