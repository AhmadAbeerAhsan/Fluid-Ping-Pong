#include "Model.hpp"

Model::Model() :
    m_model{glm::mat4(1.0f)},
    Children{}
{
    vao_id = std::make_unique<GLuint>();
    vbo_positions_id = std::make_unique<GLuint>();
    vbo_colors_id = std::make_unique<GLuint>();
    vbo_indices = std::make_unique<GLuint>();

}

Model::~Model()
{
    //std::cout << "Model deleted" << std::endl;
    glDeleteBuffers(1, vbo_positions_id.get());
    glDeleteBuffers(1, vbo_colors_id.get());
    glDeleteBuffers(1, vbo_indices.get());
    glDeleteVertexArrays(1, vao_id.get());
}

void Model::SetMaterial(std::shared_ptr<Shader> shader_ptr)
{
    m_shader_ptr = std::move(shader_ptr);
}

void Model::SetGeometry(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& colors, const std::vector<glm::uvec3>& indices)
{
    if (positions.size() == 0)
    {
        std::cout << "SetGeometry Error: Position size is 0" << std::endl;
        return;
    }
    m_positions = positions;
    
    if (colors.size() == positions.size())
    {
        m_colors = colors;
        initColors = std::function<void()>{
            [this]{
                glGenBuffers(1, vbo_colors_id.get());
                glBindBuffer(GL_ARRAY_BUFFER, *vbo_colors_id.get());
                glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec3), m_colors.data(), GL_STATIC_DRAW);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // location 1 must match your vertex shader
                glEnableVertexAttribArray(1);
            }
        };
    }
    else
    {
        m_colors = colors;
        initColors = std::function<void()>{
            [this]{}
        };
    }

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

void Model::Translate(glm::vec3 translation)
{
    m_model = glm::translate(m_model, translation);
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

    initColors();

    initIndices();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // unbind, we'll bind again in the render loop
}

void Model::draw(const glm::mat4& parent_model)
{
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
