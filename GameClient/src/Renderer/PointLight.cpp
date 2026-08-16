#include "PointLight.hpp"

PointLight::PointLight(glm::vec4 global_ambient, glm::vec4 ambient, glm::vec4 diffusion, glm::vec4 specular, glm::vec3 position):
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
    m_shader_ptrs{std::vector<std::shared_ptr<Shader>>()}
{
}

void PointLight::AddShader(std::shared_ptr<Shader> shader_ptr)
{
    m_shader_ptrs.push_back(shader_ptr);

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
}

void PointLight::PassUniforms()
{
    for (size_t i = 0; i < m_shader_ptrs.size(); i++)
    {
        glProgramUniform4fv(m_shader_ptrs[i]->ID, *m_glob_ambi_locs[i], 1, glm::value_ptr(m_global_ambient));
        glProgramUniform4fv(m_shader_ptrs[i]->ID, *m_ambi_locs[i], 1, glm::value_ptr(m_ambient));
        glProgramUniform4fv(m_shader_ptrs[i]->ID, *m_diff_locs[i], 1, glm::value_ptr(m_diffusion));
        glProgramUniform4fv(m_shader_ptrs[i]->ID, *m_spec_locs[i], 1, glm::value_ptr(m_specular));
        glProgramUniform3fv(m_shader_ptrs[i]->ID, *m_posi_locs[i], 1, glm::value_ptr(m_position));
    }
}
