#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"

class ObjectMaterial
{
private:
    /* data */
public:
    ObjectMaterial(
        glm::vec4 ambient = glm::vec4(0.2473f, 0.1995f, 0.0745f, 1),
        glm::vec4 diffusion = glm::vec4( 0.7516f, 0.6065f, 0.2265f, 1),
        glm::vec4 specular = glm::vec4(0.6283f, 0.5558f, 0.3661f, 1),
        float shininess = 51.2f
    );

    void InitializeMaterial(std::shared_ptr<Shader> shader_ptr);
    void PassUniforms();

    glm::vec4 m_ambient;
    glm::vec4 m_diffusion;
    glm::vec4 m_specular;
    float m_shininess;

    std::shared_ptr<GLuint> m_ambi_loc;
    std::shared_ptr<GLuint> m_diff_loc;
    std::shared_ptr<GLuint> m_spec_loc;
    std::shared_ptr<GLuint> m_shin_loc;

    std::shared_ptr<Shader> m_shader_ptr;
};
