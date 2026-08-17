#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"

class PointLight
{
private:
    /* data */
public:
    PointLight(
        float width, float height,
        glm::vec4 global_ambient = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f),
        glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
        glm::vec4 diffusion = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        glm::vec3 position = glm::vec3(500.0f, 200.0f, 20.0f)
    );
    
    void AddRenderShader(std::shared_ptr<Shader> shader_ptr);
    void AddShadowMapShader(std::shared_ptr<Shader> shader_ptr);
    void SetupShadowBuffers(float width, float height);
    void PassUniformsToRendererShader();
    void PassUniformsToShadowMapShader();

    void StartFillingShadowBuffer();
    void StopFillingShadowBuffer();

    void EnableShadowTexture();

    float m_width, m_height;

    glm::vec4 m_global_ambient;
    glm::vec4 m_ambient;
    glm::vec4 m_diffusion;
    glm::vec4 m_specular;
    glm::vec3 m_position;

    std::vector<std::shared_ptr<GLuint>> m_glob_ambi_locs;
    std::vector<std::shared_ptr<GLuint>> m_ambi_locs;
    std::vector<std::shared_ptr<GLuint>> m_diff_locs;
    std::vector<std::shared_ptr<GLuint>> m_spec_locs;
    std::vector<std::shared_ptr<GLuint>> m_posi_locs;
    std::vector<std::shared_ptr<GLuint>> m_proj_view_locs;
    std::vector<std::shared_ptr<Shader>> m_render_shader_ptrs;

    std::shared_ptr<GLuint> m_light_proj_loc;
    std::shared_ptr<GLuint> m_light_view_loc;
    std::shared_ptr<Shader> m_shadow_map_shader_ptr;

    std::shared_ptr<GLuint> m_shadow_tex, m_shadow_buffer;
    glm::mat4 m_light_view;
    glm::mat4 m_light_proj;
    glm::mat4 m_bias;
    glm::mat4 m_light_proj_view;
};
