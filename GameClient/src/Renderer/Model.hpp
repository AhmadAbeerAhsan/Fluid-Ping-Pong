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

struct VertexArrayDeleter
{
    void operator()(GLuint* id) const noexcept
    {
        if (id)
        {
            if (*id != 0)
            {
                std::cout << "glDeleteVertexArrays(1, id): " << *id << std::endl;
                glDeleteVertexArrays(1, id);
            }
            delete id;
        }
    }
};
 
struct BufferDeleter
{
    void operator()(GLuint* id) const noexcept
    {
        if (id)
        {
            if (*id != 0)
            {
                std::cout << "glDeleteBuffers(1, id): " << *id << std::endl;
                glDeleteBuffers(1, id);
            }
            delete id;
        }
    }
};
 
using VertexArrayUniquePtr = std::unique_ptr<GLuint, VertexArrayDeleter>;
using BufferUniquePtr      = std::unique_ptr<GLuint, BufferDeleter>;

class Model
{
public:
    Model();
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;
    Model(Model&&) = default;

    void SetShader(Shader shader_ptr);
    void SetGeometry(const std::vector<glm::vec3>& positions, const std::vector<glm::uvec3>& indices, bool is_circle = false);
    void SetMaterial(Texture texture, std::vector<glm::vec2> texCoords);
    void SetMaterial(std::vector<glm::vec2> texCoords);
    void SetMaterial(std::vector<glm::vec3> colors);
    void SetMaterial(glm::vec3 color);
    void SetMaterial(Texture texture);

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

    Shader m_shader;
    glm::vec3 m_model_positions;
    glm::vec3 m_model_rotations;
    glm::vec3 m_model_scales;
    glm::mat4 m_model;
    glm::mat4 m_temp_model;

    VertexArrayUniquePtr vao_id;
    BufferUniquePtr vbo_positions_id;
    BufferUniquePtr vbo_normals_id;
    BufferUniquePtr vbo_material_id;
    BufferUniquePtr vbo_indices;
    Texture m_texture;

    std::vector<std::shared_ptr<Model>> Children;

    bool m_send_time{false};
    bool m_enable_reflection{false};
    bool m_useColor{false};
    float m_time{0.0f};
    const glm::vec2 m_initial_dir{0.0f, 1.0f};
    glm::vec2 m_dir{0.0f, 1.0f};

    void initializeForGL();
    void DrawWithExternalShader(Shader shader, const glm::mat4& parent_model = glm::mat4(1.0f));
    void DrawWithInternalShader(const glm::mat4& parent_model = glm::mat4(1.0f));

private:
    bool m_use_indices{false};

    void ComputeNormals(bool is_circle);

};
