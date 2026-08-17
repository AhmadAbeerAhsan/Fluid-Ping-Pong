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
    void SetMaterial(const char* path, std::vector<glm::vec2> texCoords);
    void SetMaterial(std::vector<glm::vec3> colors);

    void UpdateModelMatrix();
    void Translate(glm::vec3 translation);
    void SetPosition(glm::vec3 new_position);
    void RotateX(float angle);
    void RotateY(float angle);
    void RotateZ(float angle);
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
    std::unique_ptr<Texture> m_texture;

    std::vector<std::shared_ptr<Model>> Children;

    ObjectMaterial m_object_material{};

    void initializeForGL();
    void draw(std::shared_ptr<Shader> shader_ptr, bool is_depth_shader, const glm::mat4& parent_model = glm::mat4(1.0f));

private:
    std::function<void()> initMaterial;
    std::function<void()> initIndices;
    std::function<void(int)> useTexture;
    std::function<void()> drawVertices;

    void ComputeNormals(bool is_circle);

};
