#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <functional>
#include <iostream>
#include <list>
#include <limits>

#include "Shader.hpp"
#include "Texture.hpp"
#include "ObjectMaterial.hpp"

class Model
{
public:
    Model();

    ~Model();
    Model& operator=(const Model&) = delete;

    void SetShader(std::shared_ptr<Shader> shader_ptr);
    void SetGeometry(const std::vector<glm::vec3>& positions, const std::vector<glm::uvec3>& indices, bool is_circle = false);
    void SetMaterial(std::shared_ptr<Texture> texture, std::vector<glm::vec2> texCoords);
    void SetMaterial(std::vector<glm::vec2> texCoords);
    void SetMaterial(std::vector<glm::vec3> colors);
    void SetMaterial(glm::vec3 color);
    void SetMaterial(std::shared_ptr<Texture> texture_ptr);

    void SetMaterial(std::vector<std::string> paths, std::vector<glm::vec2> texCoords);
    void ActivateTextureForOther(int gl_texPos);

    void UpdateModelMatrix();
    void Translate(glm::vec3 translation);
    void SetPosition(glm::vec3 new_position);
    void RotateX(float angle);
    void RotateY(float angle);
    void RotateZ(float angle);
    void RotateYToVectorDirection(glm::vec2 dir);
    void ScaleByFactor(float factor);
    void ScaleToMaxSize(float size);
    void ScaleDimensionToMaxSize(float size, int dimension);

    void AddChild(std::shared_ptr<Model> child);

    std::vector<glm::vec3> m_positions {};
    std::vector<glm::vec3> m_normals {};
    std::vector<glm::vec2> m_texCoords {};
    std::vector<glm::vec3> m_colors {};
    std::vector<glm::uvec3> m_indices {};

    std::shared_ptr<Shader> m_shader_ptr;
    glm::vec3 m_model_positions;
    glm::vec3 m_model_rotations;
    glm::vec3 m_model_scales;
    glm::mat4 m_model;
    glm::mat4 m_temp_model;

    std::unique_ptr<GLuint> vao_id;
    std::unique_ptr<GLuint> vbo_positions_id;
    std::unique_ptr<GLuint> vbo_normals_id;
    std::unique_ptr<GLuint> vbo_material_id;
    std::unique_ptr<GLuint> vbo_indices;
    std::shared_ptr<Texture> m_texture;

    std::vector<std::shared_ptr<Model>> Children;

    bool m_send_time{false};
    bool m_enable_reflection{false};
    bool m_useColor{false};
    float m_time{0.0f};
    const glm::vec2 m_initial_dir{0.0f, 1.0f};
    glm::vec2 m_dir{0.0f, 1.0f};

    void initializeForGL();
    void DrawWithExternalShader(std::shared_ptr<Shader> shader_ptr, const glm::mat4& parent_model = glm::mat4(1.0f));
    std::function<void(const glm::mat4&)> DrawWithInternalShader;

private:
    std::function<void()> initMaterial;
    std::function<void()> initIndices;
    std::function<void(int)> useTexture;
    std::function<void()> drawVertices;

    void ComputeNormals(bool is_circle);

};
