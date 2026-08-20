#include "Model.hpp"

Model::Model() :
    m_model{glm::mat4(1.0f)},
    m_model_positions{glm::vec3(0.0f)},
    m_model_rotations{glm::vec3(0.0f)},
    m_model_scales{glm::vec3(1.0f)},
    Children{}
{
    vao_id = std::make_unique<GLuint>();
    vbo_positions_id = std::make_unique<GLuint>();
    vbo_normals_id = std::make_unique<GLuint>();
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
    m_useColor = true;
    if (colors.size() == m_positions.size())
    {
        m_colors = colors;
        initMaterial = std::function<void()>{
            [this]{
                glGenBuffers(1, vbo_material_id.get());
                glBindBuffer(GL_ARRAY_BUFFER, *vbo_material_id.get());
                glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec3), m_colors.data(), GL_STATIC_DRAW);
                glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // location 1 must match your vertex shader
                glEnableVertexAttribArray(3);
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

void Model::SetMaterial(glm::vec3 color)
{
    m_useColor = true;
    m_colors.clear();
    m_colors.reserve(m_positions.size());
    m_colors.assign(m_positions.size(), color);

    initMaterial = std::function<void()>{
        [this]{
            glGenBuffers(1, vbo_material_id.get());
            glBindBuffer(GL_ARRAY_BUFFER, *vbo_material_id.get());
            glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec3), m_colors.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // location 1 must match your vertex shader
            glEnableVertexAttribArray(3);
        }
    };
    useTexture = std::function<void(int)>{
        [this](int gl_texPos){
        }
    };
}

void Model::SetMaterial(std::shared_ptr<Texture> texture_ptr)
{
    m_useColor = false;
    if (m_texCoords.size() != m_positions.size() && m_texCoords.size() != 0)
    {
        std::cout << "Model::SetMaterial(std::unique_ptr<Texture> texture_ptr): (m_texCoords.size() != m_positions.size() && m_texCoords.size() != 0)" << std::endl;
    }
    
    m_texture = texture_ptr;

    initMaterial = std::function<void()>{
            [this]{
                glGenBuffers(1, vbo_material_id.get());
                glBindBuffer(GL_ARRAY_BUFFER, *vbo_material_id.get());
                glBufferData(GL_ARRAY_BUFFER, m_texCoords.size() * sizeof(glm::vec2), m_texCoords.data(), GL_STATIC_DRAW);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0); // location 1 must match your vertex shader
                glEnableVertexAttribArray(2);
            }
        };
    useTexture = std::function<void(int)>{
        [this](int gl_texPos){
            m_texture->Use(gl_texPos);
        }
    };

    glm::vec3 min{std::numeric_limits<float>::max()};
    glm::vec3 max{std::numeric_limits<float>::lowest()};

    if (m_positions.size() < 1)
    {
        std::cout << "Model::ScaleToMaxSize: m_positions.size() < 1" << std::endl;
        return;
    }
 
    for (const glm::vec3& position : m_positions)
    {
        min = glm::min(min, position);
        max = glm::max(max, position);
    }
    
    float x_diff{max.x - min.x};
    float y_diff{max.y - min.y};
    float z_diff{max.z - min.z};

    glm::vec3 center{(min + max) * 0.5f};

    for (size_t i = 0; i < m_texCoords.size(); i++)
    {
        float u = 0.5f + std::atan2(m_positions[i].y - center.y, m_positions[i].x - center.x) / (2.0f * glm::pi<float>());
        float v = (m_positions[i].z - min.z) / z_diff;
        m_texCoords[i] = glm::vec2(u, v);
    }
       
}

void Model::SetMaterial(std::shared_ptr<Texture> texture, std::vector<glm::vec2> texCoords)
{
    m_useColor = false;
    if (texCoords.size() == m_positions.size())
    {
        m_texture = texture;
        m_texCoords = texCoords;
        initMaterial = std::function<void()>{
            [this]{
                glGenBuffers(1, vbo_material_id.get());
                glBindBuffer(GL_ARRAY_BUFFER, *vbo_material_id.get());
                glBufferData(GL_ARRAY_BUFFER, m_texCoords.size() * sizeof(glm::vec2), m_texCoords.data(), GL_STATIC_DRAW);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0); // location 1 must match your vertex shader
                glEnableVertexAttribArray(2);
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

void Model::SetMaterial(std::vector<glm::vec2> texCoords)
{
    m_useColor = false;
    if (texCoords.size() == m_positions.size())
    {
        m_texCoords = texCoords;
        initMaterial = std::function<void()>{
            [this]{
                glGenBuffers(1, vbo_material_id.get());
                glBindBuffer(GL_ARRAY_BUFFER, *vbo_material_id.get());
                glBufferData(GL_ARRAY_BUFFER, m_texCoords.size() * sizeof(glm::vec2), m_texCoords.data(), GL_STATIC_DRAW);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0); // location 1 must match your vertex shader
                glEnableVertexAttribArray(2);
            }
        };
        useTexture = std::function<void(int)>{
            [this](int gl_texPos){
            }
        };
    }
    else
    {
        std::cout << "texCoords.size() != m_positions.size()" << std::endl;
    }
}

void Model::SetMaterial(std::vector<std::string> paths, std::vector<glm::vec2> texCoords)
{
    m_useColor = false;
    m_texture = std::make_unique<Texture>(paths);
    m_texCoords = texCoords;
    initMaterial = std::function<void()>{
        [this]{
            // textures coord are not passed to cube maps
        }
    };
    useTexture = std::function<void(int)>{
        [this](int gl_texPos){
            m_texture->Use(gl_texPos);
        }
    };
}

void Model::ActivateTextureForOther(int gl_texPos)
{
    m_texture->Use(gl_texPos);
}

void Model::ComputeNormals(bool is_circle)
{
    m_normals.resize(m_positions.size(), glm::vec3(0.0f));

    if (is_circle)
    {
        for (size_t i = 0; i < m_positions.size(); ++i)
        {
            m_normals[i] = glm::normalize(m_positions[i]);
        }
    }
    else
    {
        if (m_indices.size() == 0)
        {
            std::cout << "Model::ComputeNormals indices not set" << std::endl;
        }
        
        for (const glm::uvec3& indices : m_indices)
        {
            const glm::vec3& p0 = m_positions[indices[0]];
            const glm::vec3& p1 = m_positions[indices[1]];
            const glm::vec3& p2 = m_positions[indices[2]];

            glm::vec3 e1 = p1 - p0;
            glm::vec3 e2 = p2 - p0;
            glm::vec3 normal = glm::cross(e1, e2);

            m_normals[indices[0]] += normal;
            m_normals[indices[1]] += normal;
            m_normals[indices[2]] += normal;
        }

        for (glm::vec3& normal : m_normals)
        {
            if (glm::dot(normal, normal) > 0.0f)
            {
                normal = glm::normalize(normal);
            }
        }
    }
}

void Model::SetGeometry(const std::vector<glm::vec3>& positions, const std::vector<glm::uvec3>& indices, bool is_circle)
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
    ComputeNormals(is_circle);
}

void Model::UpdateModelMatrix()
{
    m_model =
        glm::translate(glm::mat4(1.0f), m_model_positions) *
        glm::rotate(glm::mat4(1.0f),
                    m_model_rotations.y,
                    glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f),
                    m_model_rotations.x,
                    glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f),
                    m_model_rotations.z,
                    glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::scale(glm::mat4(1.0f), m_model_scales);
}

void Model::Translate(glm::vec3 translation)
{
    m_model_positions += translation;
}

void Model::SetPosition(glm::vec3 new_position)
{
    m_model_positions = new_position;
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

void Model::RotateYToVectorDirection(glm::vec2 dir)
{
    m_dir = glm::normalize(dir);
    m_model_rotations.y = glm::acos(glm::dot(m_dir, m_initial_dir));
    if (m_dir.x < 0)
    {
        m_model_rotations.y = -m_model_rotations.y;
    }
    
    //m_model_rotations.z = glm::acos(m_dir.y);
    UpdateModelMatrix();
}

void Model::ScaleByFactor(float factor)
{
    m_model_scales = glm::vec3(factor, factor, factor);
}

void Model::ScaleToMaxSize(float size)
{
    glm::vec3 min{std::numeric_limits<float>::max()};
    glm::vec3 max{std::numeric_limits<float>::lowest()};

    if (m_positions.size() < 1)
    {
        std::cout << "Model::ScaleToMaxSize: m_positions.size() < 1" << std::endl;
        return;
    }
 
    for (const glm::vec3& position : m_positions)
    {
        min = glm::min(min, position);
        max = glm::max(max, position);
    }
    
    float x_diff{max.x - min.x};
    float y_diff{max.y - min.y};
    float z_diff{max.z - min.z};

    float max_diff{std::max(x_diff, std::max(y_diff, z_diff))};
    float scale_factor{size/max_diff};
    m_model_scales = glm::vec3(scale_factor, scale_factor, scale_factor);
}

void Model::ScaleDimensionToMaxSize(float size, int dimension)
{
    float min{std::numeric_limits<float>::max()};
    float max{std::numeric_limits<float>::lowest()};

    if (m_positions.size() < 1)
    {
        std::cout << "Model::ScaleToMaxSize: m_positions.size() < 1" << std::endl;
        return;
    }
 
    for (const glm::vec3& position : m_positions)
    {
        min = std::min(min, position[dimension]);
        max = std::max(max, position[dimension]);
    }

    float diff{max - min};
    float scale_factor{size/diff};
    m_model_scales = glm::vec3(scale_factor, scale_factor, scale_factor);
}

void Model::AddChild(std::shared_ptr<Model> child)
{
    Children.emplace_back(child);
}

void Model::initializeForGL()
{
    glGenVertexArrays(1, vao_id.get());
    glGenBuffers(1, vbo_positions_id.get());
    glGenBuffers(1, vbo_normals_id.get());

    glBindVertexArray(*vao_id.get());

    glBindBuffer(GL_ARRAY_BUFFER, *vbo_positions_id.get());
    glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // location 0 must match your vertex shader
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, *vbo_normals_id.get());
    glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // location 1 must match your vertex shader
    glEnableVertexAttribArray(1);

    initMaterial();

    initIndices();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // unbind, we'll bind again in the render loop

    DrawWithInternalShader = std::function<void(const glm::mat4&)>{
        [this](const glm::mat4& parent_model){
            useTexture(GL_TEXTURE0);

            m_temp_model = parent_model * m_model;
            
            m_shader_ptr->setMat4("model", m_temp_model);
            m_shader_ptr->setBool("send_time", m_send_time);
            m_shader_ptr->setFloat("time", m_time);
            m_shader_ptr->setBool("enableReflection", m_enable_reflection);
            m_shader_ptr->setBool("useColor", m_useColor);

            glBindVertexArray(*vao_id.get());
            drawVertices();
            glBindVertexArray(0);

            for (std::shared_ptr<Model>& child : Children)
            {
                child->DrawWithInternalShader(m_temp_model);
            }
        }
    };
}

void Model::DrawWithExternalShader(std::shared_ptr<Shader> shader_ptr, const glm::mat4& parent_model)
{
    useTexture(GL_TEXTURE0);

    m_temp_model = parent_model * m_model;
    
    shader_ptr->setMat4("model", m_temp_model);
    
    glBindVertexArray(*vao_id.get());
    drawVertices();
    glBindVertexArray(0);

    for (std::shared_ptr<Model>& child : Children)
    {
        child->DrawWithExternalShader(shader_ptr, m_temp_model);
    }
}
