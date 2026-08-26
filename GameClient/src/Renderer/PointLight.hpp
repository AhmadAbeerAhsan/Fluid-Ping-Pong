#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Framebuffer.hpp"

class PointLight
{
private:
    /* data */
public:
    PointLight(
        std::shared_ptr<glm::ivec2> shared_resolution,
        glm::vec4 global_ambient = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f),
        glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
        glm::vec4 diffusion = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        glm::vec3 position = glm::vec3(-400.0f, 300.0f, -400.0f)
    );
    
    void SetRenderShader(Shader shader);
    void PassUniformsToRendererShader();
    
    void SetShadowMapShader(Shader shader);

    void StartFillingShadowBuffer();
    void StopFillingShadowBuffer();

    void EnableShadowTexture();

    void Resize();

    std::shared_ptr<glm::ivec2> m_shared_resolution;

    glm::vec4 m_global_ambient;
    glm::vec4 m_ambient;
    glm::vec4 m_diffusion;
    glm::vec4 m_specular;
    glm::vec3 m_position;

    GLint m_glob_ambi_loc;
    GLint m_ambi_loc;
    GLint m_diff_loc;
    GLint m_spec_loc;
    GLint m_posi_loc;
    GLint m_proj_view_loc;
    Shader m_render_shader;

    GLint m_light_proj_loc;
    GLint m_light_view_loc;
    Shader m_shadow_map_shader;

    Framebuffer shadowBuffer;

    glm::mat4 m_light_view;
    glm::mat4 m_light_proj;
    glm::mat4 m_bias;
    glm::mat4 m_light_proj_view;
};
