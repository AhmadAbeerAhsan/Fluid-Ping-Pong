#include "Model.hpp"

Model::Model() :
    m_model{glm::mat4(1.0f)},
    m_model_positions{glm::vec3(0.0f)},
    m_model_rotations{glm::vec3(0.0f)},
    Children{}
{
    vao_id = std::make_unique<GLuint>();
    vbo_positions_id = std::make_unique<GLuint>();
    vbo_material_id = std::make_unique<GLuint>();
    vbo_indices = std::make_unique<GLuint>();
}

Model::~Model()
{
    //std::cout << "Model deleted" << std::endl;
    glDeleteBuffers(1, vbo_positions_id.get());
    glDeleteBuffers(1, vbo_material_id.get());
    glDeleteBuffers(1, vbo_indices.get());
    glDeleteVertexArrays(1, vao_id.get());
}

void Model::SetShader(std::shared_ptr<Shader> shader_ptr)
{
    m_shader_ptr = std::move(shader_ptr);
}

void Model::SetMaterial(std::vector<glm::vec3> colors)
{
    if (colors.size() == m_positions.size())
    {
        m_colors = colors;
        initMaterial = std::function<void()>{
            [this]{
                glGenBuffers(1, vbo_material_id.get());
                glBindBuffer(GL_ARRAY_BUFFER, *vbo_material_id.get());
                glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec3), m_colors.data(), GL_STATIC_DRAW);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // location 1 must match your vertex shader
                glEnableVertexAttribArray(1);
            }
        };
        useTexture = std::function<void(int)>{
            [this](int gl_texPos){
            }
        };
    }
    else
    {
        std::cout << "colors.size() != m_positions.size()" << std::endl;
    }
}

void Model::SetMaterial(const char* path, std::vector<glm::vec2> texCoords)
{
    if (texCoords.size() == m_positions.size())
    {
        m_texture = std::make_unique<Texture>(path);
        m_texCoords = texCoords;
        initMaterial = std::function<void()>{
            [this]{
                glGenBuffers(1, vbo_material_id.get());
                glBindBuffer(GL_ARRAY_BUFFER, *vbo_material_id.get());
                glBufferData(GL_ARRAY_BUFFER, m_texCoords.size() * sizeof(glm::vec2), m_texCoords.data(), GL_STATIC_DRAW);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0); // location 1 must match your vertex shader
                glEnableVertexAttribArray(1);
            }
        };
        useTexture = std::function<void(int)>{
            [this](int gl_texPos){
                m_texture->Use(gl_texPos);
            }
        };
    }
    else
    {
        std::cout << "texCoords.size() != m_positions.size()" << std::endl;
    }
}

void Model::SetGeometry(const std::vector<glm::vec3>& positions, const std::vector<glm::uvec3>& indices)
{
    if (positions.size() == 0)
    {
        std::cout << "SetGeometry Error: Position size is 0" << std::endl;
        return;
    }
    m_positions = positions;

    if (indices.size() == 0)
    {
        initIndices = std::function<void()>{
            [this]{}
        };

        drawVertices = std::function<void()>{
            [this]{
                glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_positions.size()));
            }
        };
    }
    else
    {
        m_indices = indices;
        initIndices = std::function<void()>{
            [this]{
                glGenBuffers(1, vbo_indices.get());
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *vbo_indices.get());
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(glm::uvec3), m_indices.data(), GL_STATIC_DRAW);
            }
        };

        drawVertices = std::function<void()>{
            [this]{
                glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size() * 3), GL_UNSIGNED_INT, 0);
            }
        };
    }
    
    
}

void UpdateModelMatrix();

// Model.cpp
void Model::UpdateModelMatrix()
{
    m_model =
        glm::translate(glm::mat4(1.0f), m_model_positions) *
        glm::rotate(glm::mat4(1.0f),
                    glm::radians(m_model_rotations.y),
                    glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f),
                    glm::radians(m_model_rotations.x),
                    glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f),
                    glm::radians(m_model_rotations.z),
                    glm::vec3(0.0f, 0.0f, 1.0f));
}

void Model::Translate(glm::vec3 translation)
{
    m_model_positions += translation;
}

void Model::RotateX(float angle)
{
    m_model_rotations.x += angle;
}

void Model::RotateY(float angle)
{
    m_model_rotations.y += angle;
}

void Model::RotateZ(float angle)
{
    m_model_rotations.z += angle;
}

void Model::AddChild(std::shared_ptr<Model> child)
{
    Children.emplace_back(child);
}

void Model::initializeForGL()
{
    glGenVertexArrays(1, vao_id.get());
    glGenBuffers(1, vbo_positions_id.get());

    glBindVertexArray(*vao_id.get());

    glBindBuffer(GL_ARRAY_BUFFER, *vbo_positions_id.get());
    glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // location 0 must match your vertex shader
    glEnableVertexAttribArray(0);

    initMaterial();

    initIndices();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // unbind, we'll bind again in the render loop
}

void Model::draw(const glm::mat4& parent_model)
{
    useTexture(GL_TEXTURE0);

    m_shader_ptr->use();
    m_temp_model = parent_model * m_model;
    m_shader_ptr->setMat4("model", m_temp_model);

    glBindVertexArray(*vao_id.get());
    drawVertices();
    glBindVertexArray(0);

    for (std::shared_ptr<Model>& child : Children)
    {
        child->draw(m_temp_model);
    }
    
}
