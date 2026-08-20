#include "PointLight.hpp"

PointLight::PointLight(std::shared_ptr<glm::ivec2> shared_resolution, glm::vec4 global_ambient, glm::vec4 ambient, glm::vec4 diffusion, glm::vec4 specular, glm::vec3 position):
    m_global_ambient(global_ambient),
    m_ambient(ambient),
    m_diffusion(diffusion),
    m_specular(specular),
    m_position(position),
    m_glob_ambi_loc{std::make_shared<GLuint>()},
    m_ambi_loc{std::make_shared<GLuint>()},
    m_diff_loc{std::make_shared<GLuint>()},
    m_spec_loc{std::make_shared<GLuint>()},
    m_posi_loc{std::make_shared<GLuint>()},
    m_proj_view_loc{std::make_shared<GLuint>()},
    m_shared_resolution(shared_resolution),
    shadowBuffer(Framebuffer::FrameBufferType::Shadow_Map, shared_resolution)
{
    m_light_proj = glm::perspective(
        glm::radians(8.0f),
        (float)m_shared_resolution->x / (float)m_shared_resolution->y,
        0.1f,
        10000.0f
    );
    
    glm::vec3 target(0.0f);
    glm::vec3 direction = glm::normalize(target - m_position);

    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

    m_light_view = glm::lookAt(
        m_position,
        target,
        worldUp
    );

    m_bias = glm::mat4(
        0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f
    );

    m_light_proj_view = m_bias * m_light_proj * m_light_view;
}

void PointLight::SetRenderShader(std::shared_ptr<Shader> shader_ptr)
{
    m_render_shader_ptr = shader_ptr;

    m_glob_ambi_loc = std::make_shared<GLuint>( glGetUniformLocation(shader_ptr->ID, "light.global_ambient") );
    m_ambi_loc = std::make_shared<GLuint>( glGetUniformLocation(shader_ptr->ID, "light.ambient") );
    m_diff_loc = std::make_shared<GLuint>( glGetUniformLocation(shader_ptr->ID, "light.diffuse") );
    m_spec_loc = std::make_shared<GLuint>( glGetUniformLocation(shader_ptr->ID, "light.specular") );
    m_posi_loc = std::make_shared<GLuint>( glGetUniformLocation(shader_ptr->ID, "light.position") );
    m_proj_view_loc = std::make_shared<GLuint>( glGetUniformLocation(shader_ptr->ID, "light.proj_view") );

    GLint shTexLoc = glGetUniformLocation(shader_ptr->ID, "shTex");
    GLint ourTexLoc = glGetUniformLocation(shader_ptr->ID, "ourTexture");

    std::cout << "[" << shader_ptr->ID << "] shTex loc = " << shTexLoc
              << ", ourTexture loc = " << ourTexLoc << std::endl;

    //if (shTexLoc == -1)
        //std::cout << "WARNING: 'shTex' uniform not found — check spelling or whether it's optimized out (unused in a code path)" << std::endl;

    glProgramUniform1i(shader_ptr->ID, shTexLoc, 1);   // must match GL_TEXTURE1
    glProgramUniform1i(shader_ptr->ID, ourTexLoc, 0);  // must match GL_TEXTURE0
}

void PointLight::PassUniformsToRendererShader()
{
    glProgramUniform4fv(m_render_shader_ptr->ID, *m_glob_ambi_loc, 1, glm::value_ptr(m_global_ambient));
    glProgramUniform4fv(m_render_shader_ptr->ID, *m_ambi_loc, 1, glm::value_ptr(m_ambient));
    glProgramUniform4fv(m_render_shader_ptr->ID, *m_diff_loc, 1, glm::value_ptr(m_diffusion));
    glProgramUniform4fv(m_render_shader_ptr->ID, *m_spec_loc, 1, glm::value_ptr(m_specular));
    glProgramUniform3fv(m_render_shader_ptr->ID, *m_posi_loc, 1, glm::value_ptr(m_position));
    glProgramUniformMatrix4fv(m_render_shader_ptr->ID, *m_proj_view_loc, 1, GL_FALSE, glm::value_ptr(m_light_proj_view));
}

void PointLight::SetShadowMapShader(std::shared_ptr<Shader> shader_ptr)
{
    m_shadow_map_shader_ptr = shader_ptr;

    m_light_proj_loc = std::make_shared<GLuint>(
        glGetUniformLocation(m_shadow_map_shader_ptr->ID, "shadow_proj")
    );
    m_light_view_loc = std::make_shared<GLuint>(
        glGetUniformLocation(m_shadow_map_shader_ptr->ID, "shadow_view")
    );

    m_shadow_map_shader_ptr->PassUniforms = std::function<void()>{
        [this](){
            glProgramUniformMatrix4fv(m_shadow_map_shader_ptr->ID, *m_light_proj_loc, 1, GL_FALSE, glm::value_ptr(m_light_proj));
            glProgramUniformMatrix4fv(m_shadow_map_shader_ptr->ID, *m_light_view_loc, 1, GL_FALSE, glm::value_ptr(m_light_view));
        }
    };
}

void PointLight::StartFillingShadowBuffer()
{
    //make the custom framebuffer current, and associate it with the shadow texture
    shadowBuffer.Bind();
    shadowBuffer.UnBindTexture();

    //disable drawing colors, but enable the depth computation
    glDrawBuffer(GL_NONE);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0f, 4.0f);

    m_shadow_map_shader_ptr->Activate();
    m_shadow_map_shader_ptr->PassUniforms();

    //pass all models that cast shadows
}

void PointLight::StopFillingShadowBuffer()
{
    glDisable(GL_POLYGON_OFFSET_FILL);

    //after all models passed
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    //glDrawBuffer(GL_FRONT); // re-enables drawing colors
    glDrawBuffer(GL_BACK);
}

void PointLight::EnableShadowTexture()
{
    shadowBuffer.BindTexture(GL_TEXTURE1);
    
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

void PointLight::Resize()
{
    shadowBuffer.Resize();
    m_light_proj = glm::perspective(
        glm::radians(8.0f),
        (float)m_shared_resolution->x / (float)m_shared_resolution->y,
        0.1f,
       10000.0f
    );
    m_light_proj_view = m_bias * m_light_proj * m_light_view;
}
