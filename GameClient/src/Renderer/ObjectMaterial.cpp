#include "ObjectMaterial.hpp"

ObjectMaterial::ObjectMaterial(glm::vec4 ambient, glm::vec4 diffusion, glm::vec4 specular, float shininess) :
    m_ambient(ambient),
    m_diffusion(diffusion),
    m_specular(specular),
    m_shininess(shininess)
{
    m_ambi_loc = std::make_unique<GLuint>();
    m_diff_loc = std::make_unique<GLuint>();
    m_spec_loc = std::make_unique<GLuint>();
    m_shin_loc = std::make_unique<GLuint>();

    m_shader_ptr = std::make_unique<Shader>();
}

void ObjectMaterial::InitializeMaterial(std::shared_ptr<Shader> shader_ptr)
{
    m_shader_ptr = shader_ptr;

    *m_ambi_loc = glGetUniformLocation(m_shader_ptr->ID, "material.ambient");
    *m_diff_loc = glGetUniformLocation(m_shader_ptr->ID, "material.diffuse");
    *m_spec_loc = glGetUniformLocation(m_shader_ptr->ID, "material.specular");
    *m_shin_loc = glGetUniformLocation(m_shader_ptr->ID, "material.shininess");
}

void ObjectMaterial::PassUniforms()
{
    glProgramUniform4fv(m_shader_ptr->ID, *m_ambi_loc, 1, glm::value_ptr(m_ambient));
    glProgramUniform4fv(m_shader_ptr->ID, *m_diff_loc, 1, glm::value_ptr(m_diffusion));
    glProgramUniform4fv(m_shader_ptr->ID, *m_spec_loc, 1, glm::value_ptr(m_specular));
    glProgramUniform1f(m_shader_ptr->ID, *m_shin_loc, m_shininess);
}
