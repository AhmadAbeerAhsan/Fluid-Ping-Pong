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

#include "Shader.hpp"

class Model
{
public:
    Model();

    ~Model();
    Model& operator=(const Model&) = delete;

    void SetMaterial(std::shared_ptr<Shader> shader_ptr);
    void SetGeometry(const std::vector<glm::vec3>& positions = {}, const std::vector<glm::vec3>& colors = {}, const std::vector<glm::uvec3>& indices = {});

    void Translate(glm::vec3 translation);
    void AddChild(std::shared_ptr<Model> child);

    std::vector<glm::vec3> m_positions {};
    std::vector<glm::vec3> m_colors {};
    std::vector<glm::uvec3> m_indices {};

    std::shared_ptr<Shader> m_shader_ptr;
    glm::mat4 m_model;
    glm::mat4 m_temp_model;

    std::unique_ptr<GLuint> vao_id;
    std::unique_ptr<GLuint> vbo_positions_id;
    std::unique_ptr<GLuint> vbo_colors_id;
    std::unique_ptr<GLuint> vbo_indices;

    std::vector<std::shared_ptr<Model>> Children;

    void initializeForGL();
    void draw(const glm::mat4& parent_model = glm::mat4(1.0f));

private:
    std::function<void()> initColors;
    std::function<void()> initIndices;
    std::function<void()> drawVertices;
};
