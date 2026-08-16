#include "PointLight.hpp"

PointLight::PointLight(float width, float height, glm::vec4 global_ambient, glm::vec4 ambient, glm::vec4 diffusion, glm::vec4 specular, glm::vec3 position):
    m_global_ambient(global_ambient),
    m_ambient(ambient),
    m_diffusion(diffusion),
    m_specular(specular),
    m_position(position),
    m_glob_ambi_locs{std::vector<std::shared_ptr<GLuint>>()},
    m_ambi_locs{std::vector<std::shared_ptr<GLuint>>()},
    m_diff_locs{std::vector<std::shared_ptr<GLuint>>()},
    m_spec_locs{std::vector<std::shared_ptr<GLuint>>()},
    m_posi_locs{std::vector<std::shared_ptr<GLuint>>()},
    m_proj_view_locs{std::vector<std::shared_ptr<GLuint>>()},
    m_render_shader_ptrs{std::vector<std::shared_ptr<Shader>>()},
    m_shadow_tex(std::make_shared<GLuint>()),
    m_shadow_buffer(std::make_shared<GLuint>()),
    m_width(width),
    m_height(height)
{
    m_light_proj = glm::perspective(
        glm::radians(8.0f),
        m_width/m_height,
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

void PointLight::AddRenderShader(std::shared_ptr<Shader> shader_ptr)
{
    m_render_shader_ptrs.push_back(shader_ptr);

    m_glob_ambi_locs.push_back( std::make_shared<GLuint>(
        glGetUniformLocation(shader_ptr->ID, "light.global_ambient")
    ) );
    m_ambi_locs.push_back( std::make_shared<GLuint>(
        glGetUniformLocation(shader_ptr->ID, "light.ambient")
    ) );
    m_diff_locs.push_back( std::make_shared<GLuint>(
        glGetUniformLocation(shader_ptr->ID, "light.diffuse")
    ) );
    m_spec_locs.push_back( std::make_shared<GLuint>(
        glGetUniformLocation(shader_ptr->ID, "light.specular")
    ) );
    m_posi_locs.push_back( std::make_shared<GLuint>(
        glGetUniformLocation(shader_ptr->ID, "light.position")
    ) );
    m_proj_view_locs.push_back( std::make_shared<GLuint>(
        glGetUniformLocation(shader_ptr->ID, "light.proj_view")
    ) );

    GLint shTexLoc = glGetUniformLocation(shader_ptr->ID, "shTex");
    GLint ourTexLoc = glGetUniformLocation(shader_ptr->ID, "ourTexture");

    std::cout << "[" << shader_ptr->ID << "] shTex loc = " << shTexLoc
              << ", ourTexture loc = " << ourTexLoc << std::endl;

    if (shTexLoc == -1)
        std::cout << "WARNING: 'shTex' uniform not found — check spelling or whether it's optimized out (unused in a code path)" << std::endl;

    glProgramUniform1i(shader_ptr->ID, shTexLoc, 1);   // must match GL_TEXTURE1
    glProgramUniform1i(shader_ptr->ID, ourTexLoc, 0);  // must match GL_TEXTURE0
}

void PointLight::AddShadowMapShader(std::shared_ptr<Shader> shader_ptr)
{
    m_shadow_map_shader_ptr = shader_ptr;

    m_light_proj_loc = std::make_shared<GLuint>(
        glGetUniformLocation(m_shadow_map_shader_ptr->ID, "shadow_proj")
    );
    m_light_view_loc = std::make_shared<GLuint>(
        glGetUniformLocation(m_shadow_map_shader_ptr->ID, "shadow_view")
    );
}

void PointLight::PassUniformsToRendererShader()
{
    for (size_t i = 0; i < m_render_shader_ptrs.size(); i++)
    {
        glProgramUniform4fv(m_render_shader_ptrs[i]->ID, *m_glob_ambi_locs[i], 1, glm::value_ptr(m_global_ambient));
        glProgramUniform4fv(m_render_shader_ptrs[i]->ID, *m_ambi_locs[i], 1, glm::value_ptr(m_ambient));
        glProgramUniform4fv(m_render_shader_ptrs[i]->ID, *m_diff_locs[i], 1, glm::value_ptr(m_diffusion));
        glProgramUniform4fv(m_render_shader_ptrs[i]->ID, *m_spec_locs[i], 1, glm::value_ptr(m_specular));
        glProgramUniform3fv(m_render_shader_ptrs[i]->ID, *m_posi_locs[i], 1, glm::value_ptr(m_position));
        glProgramUniformMatrix4fv(m_render_shader_ptrs[i]->ID, *m_proj_view_locs[i], 1, GL_FALSE, glm::value_ptr(m_light_proj_view));
    }
}

void PointLight::PassUniformsToShadowMapShader()
{
    glProgramUniformMatrix4fv(m_shadow_map_shader_ptr->ID, *m_light_proj_loc, 1, GL_FALSE, glm::value_ptr(m_light_proj));
    glProgramUniformMatrix4fv(m_shadow_map_shader_ptr->ID, *m_light_view_loc, 1, GL_FALSE, glm::value_ptr(m_light_view));
}

void PointLight::StartFillingShadowBuffer()
{
    //make the custom framebuffer current, and associate it with the shadow texture
    glBindFramebuffer(GL_FRAMEBUFFER, *m_shadow_buffer);
    glViewport(0, 0, static_cast<GLsizei>(m_width), static_cast<GLsizei>(m_height)); 
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, *m_shadow_tex, 0);

    //disable drawing colors, but enable the depth computation
    glDrawBuffer(GL_NONE);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0f, 4.0f);

    m_shadow_map_shader_ptr->use();
    PassUniformsToShadowMapShader();

    //pass all models that cast shadows
}

void PointLight::StopFillingShadowBuffer()
{
    glEnable(GL_POLYGON_OFFSET_FILL);

    //after all models passed
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, *m_shadow_tex);
    
    //glDrawBuffer(GL_FRONT); // re-enables drawing colors
    glDrawBuffer(GL_BACK);
}

void PointLight::SetupShadowBuffers(float width, float height)
{
    glGenFramebuffers(1, m_shadow_buffer.get());

    glGenTextures(1, m_shadow_tex.get());
    glBindTexture(GL_TEXTURE_2D, *m_shadow_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
        width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    // may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
