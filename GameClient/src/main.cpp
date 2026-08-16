#define STB_IMAGE_IMPLEMENTATION

#include "Renderer/Renderer.hpp"
#include "Renderer/Model.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/ModelLoader.hpp"
#include "Renderer/PointLight.hpp"

#include <cmath>
float CalculateDiagonalLength(float b, float p)
{
    return std::sqrt(b * b + p * p);
}

void GenerateCube(
    std::vector<glm::vec3>& m_positions,
    std::vector<glm::vec3>& m_colors,
    std::vector<glm::uvec3>& m_indices
)
{
    m_positions.emplace_back(glm::vec3(-1.0, -1.0, 1.0));
    m_positions.emplace_back(glm::vec3(1.0, -1.0, 1.0));
    m_positions.emplace_back(glm::vec3(-1.0, 1.0, 1.0));
    m_positions.emplace_back(glm::vec3(1.0, 1.0, 1.0));
    m_positions.emplace_back(glm::vec3(-1.0, -1.0, -1.0));
    m_positions.emplace_back(glm::vec3(1.0, -1.0, -1.0));
    m_positions.emplace_back(glm::vec3(-1.0, 1.0, -1.0));
    m_positions.emplace_back(glm::vec3(1.0, 1.0, -1.0));

    m_colors.emplace_back(glm::vec3(0.0, 0.0, 1.0));
    m_colors.emplace_back(glm::vec3(1.0, 0.0, 1.0));
    m_colors.emplace_back(glm::vec3(0.0, 1.0, 1.0));
    m_colors.emplace_back(glm::vec3(1.0, 1.0, 1.0));
    m_colors.emplace_back(glm::vec3(0.0, 0.0, 0.0));
    m_colors.emplace_back(glm::vec3(1.0, 0.0, 0.0));
    m_colors.emplace_back(glm::vec3(0.0, 1.0, 0.0));
    m_colors.emplace_back(glm::vec3(1.0, 1.0, 0.0));

    m_indices.emplace_back(glm::uvec3(0,1,2));
    m_indices.emplace_back(glm::uvec3(2,1,3));
    m_indices.emplace_back(glm::uvec3(6,5,4));
    m_indices.emplace_back(glm::uvec3(7,5,6));

    m_indices.emplace_back(glm::uvec3(0,2,4));
    m_indices.emplace_back(glm::uvec3(2,6,4));
    m_indices.emplace_back(glm::uvec3(1,5,3));
    m_indices.emplace_back(glm::uvec3(3,5,7));

    m_indices.emplace_back(glm::uvec3(2,3,6));
    m_indices.emplace_back(glm::uvec3(6,3,7));
    m_indices.emplace_back(glm::uvec3(0,4,1));
    m_indices.emplace_back(glm::uvec3(4,5,1));
}

void GenerateRectanle(
    std::vector<glm::vec3>& m_positions,
    std::vector<glm::vec3>& m_colors,
    std::vector<glm::uvec3>& m_indices,
    float x_size, float y_size, float z_size
)
{
    m_positions.emplace_back(glm::vec3(-1.0 * (x_size/2.0f), -1.0 * (y_size/2.0f), 1.0 * (z_size/2.0f)));
    m_positions.emplace_back(glm::vec3(1.0 * (x_size/2.0f), -1.0 * (y_size/2.0f), 1.0 * (z_size/2.0f)));
    m_positions.emplace_back(glm::vec3(-1.0 * (x_size/2.0f), 1.0 * (y_size/2.0f), 1.0 * (z_size/2.0f)));
    m_positions.emplace_back(glm::vec3(1.0 * (x_size/2.0f), 1.0 * (y_size/2.0f), 1.0 * (z_size/2.0f)));
    m_positions.emplace_back(glm::vec3(-1.0 * (x_size/2.0f), -1.0 * (y_size/2.0f), -1.0 * (z_size/2.0f)));
    m_positions.emplace_back(glm::vec3(1.0 * (x_size/2.0f), -1.0 * (y_size/2.0f), -1.0 * (z_size/2.0f)));
    m_positions.emplace_back(glm::vec3(-1.0 * (x_size/2.0f), 1.0 * (y_size/2.0f), -1.0 * (z_size/2.0f)));
    m_positions.emplace_back(glm::vec3(1.0 * (x_size/2.0f), 1.0 * (y_size/2.0f), -1.0 * (z_size/2.0f)));

    m_colors.emplace_back(glm::vec3(0.0, 0.0, 1.0));
    m_colors.emplace_back(glm::vec3(1.0, 0.0, 1.0));
    m_colors.emplace_back(glm::vec3(0.0, 1.0, 1.0));
    m_colors.emplace_back(glm::vec3(1.0, 1.0, 1.0));
    m_colors.emplace_back(glm::vec3(0.0, 0.0, 0.0));
    m_colors.emplace_back(glm::vec3(1.0, 0.0, 0.0));
    m_colors.emplace_back(glm::vec3(0.0, 1.0, 0.0));
    m_colors.emplace_back(glm::vec3(1.0, 1.0, 0.0));

    m_indices.emplace_back(glm::uvec3(0,1,2));
    m_indices.emplace_back(glm::uvec3(2,1,3));
    m_indices.emplace_back(glm::uvec3(6,5,4));
    m_indices.emplace_back(glm::uvec3(7,5,6));

    m_indices.emplace_back(glm::uvec3(0,2,4));
    m_indices.emplace_back(glm::uvec3(2,6,4));
    m_indices.emplace_back(glm::uvec3(1,5,3));
    m_indices.emplace_back(glm::uvec3(3,5,7));

    m_indices.emplace_back(glm::uvec3(2,3,6));
    m_indices.emplace_back(glm::uvec3(6,3,7));
    m_indices.emplace_back(glm::uvec3(0,4,1));
    m_indices.emplace_back(glm::uvec3(4,5,1));
}

void GenerateTexturedRectanle(
    std::vector<glm::vec3>& m_positions,
    std::vector<glm::vec2>& m_tex_coords,
    std::vector<glm::uvec3>& m_indices,
    float x_size, float y_size, float z_size
)
{
    m_positions = {
        // Front
        {-1.0f * (x_size/2.0f), -1.0f * (y_size/2.0f),  1.0f * (z_size/2.0f)},
        { 1.0f * (x_size/2.0f), -1.0f * (y_size/2.0f),  1.0f * (z_size/2.0f)},
        { 1.0f * (x_size/2.0f),  1.0f * (y_size/2.0f),  1.0f * (z_size/2.0f)},
        {-1.0f * (x_size/2.0f),  1.0f * (y_size/2.0f),  1.0f * (z_size/2.0f)},

        // Back
        { 1.0f * (x_size/2.0f), -1.0f * (y_size/2.0f), -1.0f * (z_size/2.0f)},
        {-1.0f * (x_size/2.0f), -1.0f * (y_size/2.0f), -1.0f * (z_size/2.0f)},
        {-1.0f * (x_size/2.0f),  1.0f * (y_size/2.0f), -1.0f * (z_size/2.0f)},
        { 1.0f * (x_size/2.0f),  1.0f * (y_size/2.0f), -1.0f * (z_size/2.0f)},

        // Left
        {-1.0f * (x_size/2.0f), -1.0f * (y_size/2.0f), -1.0f * (z_size/2.0f)},
        {-1.0f * (x_size/2.0f), -1.0f * (y_size/2.0f),  1.0f * (z_size/2.0f)},
        {-1.0f * (x_size/2.0f),  1.0f * (y_size/2.0f),  1.0f * (z_size/2.0f)},
        {-1.0f * (x_size/2.0f),  1.0f * (y_size/2.0f), -1.0f * (z_size/2.0f)},

        // Right
        { 1.0f * (x_size/2.0f), -1.0f * (y_size/2.0f),  1.0f * (z_size/2.0f)},
        { 1.0f * (x_size/2.0f), -1.0f * (y_size/2.0f), -1.0f * (z_size/2.0f)},
        { 1.0f * (x_size/2.0f),  1.0f * (y_size/2.0f), -1.0f * (z_size/2.0f)},
        { 1.0f * (x_size/2.0f),  1.0f * (y_size/2.0f),  1.0f * (z_size/2.0f)},

        // Top
        {-1.0f * (x_size/2.0f),  1.0f * (y_size/2.0f),  1.0f * (z_size/2.0f)},
        { 1.0f * (x_size/2.0f),  1.0f * (y_size/2.0f),  1.0f * (z_size/2.0f)},
        { 1.0f * (x_size/2.0f),  1.0f * (y_size/2.0f), -1.0f * (z_size/2.0f)},
        {-1.0f * (x_size/2.0f),  1.0f * (y_size/2.0f), -1.0f * (z_size/2.0f)},

        // Bottom
        {-1.0f * (x_size/2.0f), -1.0f * (y_size/2.0f), -1.0f * (z_size/2.0f)},
        { 1.0f * (x_size/2.0f), -1.0f * (y_size/2.0f), -1.0f * (z_size/2.0f)},
        { 1.0f * (x_size/2.0f), -1.0f * (y_size/2.0f),  1.0f * (z_size/2.0f)},
        {-1.0f * (x_size/2.0f), -1.0f * (y_size/2.0f),  1.0f * (z_size/2.0f)}
    };

    for (int i = 0; i < 6; ++i)
    {
        m_tex_coords.emplace_back(0.0f, 0.0f);
        m_tex_coords.emplace_back(1.0f, 0.0f);
        m_tex_coords.emplace_back(1.0f, 1.0f);
        m_tex_coords.emplace_back(0.0f, 1.0f);
    }

    m_indices = {
        // Front
        { 0, 1, 2 },
        { 0, 2, 3 },

        // Back
        { 4, 5, 6 },
        { 4, 6, 7 },

        // Left
        { 8, 9, 10 },
        { 8, 10, 11 },

        // Right
        { 12, 13, 14 },
        { 12, 14, 15 },

        // Top
        { 16, 17, 18 },
        { 16, 18, 19 },

        // Bottom
        { 20, 21, 22 },
        { 20, 22, 23 }
    };
}

void GenerateXZBase(
    std::vector<glm::vec3>& m_positions,
    std::vector<glm::vec3>& m_colors,
    std::vector<glm::uvec3>& m_indices,
    std::vector<glm::vec2>& m_tex_coords,
    float x_size, float z_size,
    float goal_size, float side_border_size
)
{
    //bottom left to top right
    m_positions.emplace_back(glm::vec3(-1.0 * goal_size/2.0f, 0, -1.0 * (z_size/2.0f)));
    m_positions.emplace_back(glm::vec3(1.0 * goal_size/2.0f, 0, -1.0 * (z_size/2.0f)));

    m_positions.emplace_back(glm::vec3(-1.0 * (x_size/2.0f), 0, -1.0 * (side_border_size/2.0f)));
    m_positions.emplace_back(glm::vec3(1.0 * (x_size/2.0f), 0, -1.0 * (side_border_size/2.0f)));
    m_positions.emplace_back(glm::vec3(-1.0 * (x_size/2.0f), 0, 1.0 * (side_border_size/2.0f)));
    m_positions.emplace_back(glm::vec3(1.0 * (x_size/2.0f), 0, 1.0 * (side_border_size/2.0f)));

    m_positions.emplace_back(glm::vec3(-1.0 * goal_size/2.0f, 0, 1.0 * (z_size/2.0f)));
    m_positions.emplace_back(glm::vec3(1.0 * goal_size/2.0f, 0, 1.0 * (z_size/2.0f)));

    m_tex_coords.emplace_back(glm::vec2(0.0, ((x_size - goal_size)/2.0f)/x_size));
    m_tex_coords.emplace_back(glm::vec2(0.0, 1.0f - ((x_size - goal_size)/2.0f)/x_size));

    m_tex_coords.emplace_back(glm::vec2(((z_size - side_border_size)/2.0f)/z_size, 0.0));
    m_tex_coords.emplace_back(glm::vec2(((z_size - side_border_size)/2.0f)/z_size, 1.0));
    m_tex_coords.emplace_back(glm::vec2(1.0f - ((z_size - side_border_size)/2.0f)/z_size, 0.0));
    m_tex_coords.emplace_back(glm::vec2(1.0f - ((z_size - side_border_size)/2.0f)/z_size, 1.0));

    m_tex_coords.emplace_back(glm::vec2(1.0, ((x_size - goal_size)/2.0f)/x_size));
    m_tex_coords.emplace_back(glm::vec2(1.0, 1.0f - ((x_size - goal_size)/2.0f)/x_size));

    m_colors.emplace_back(glm::vec3(1.0, 0.0, 0.0));
    m_colors.emplace_back(glm::vec3(1.0, 0.0, 0.0));
    m_colors.emplace_back(glm::vec3(1.0, 0.0, 0.0));
    m_colors.emplace_back(glm::vec3(1.0, 0.0, 0.0));
    m_colors.emplace_back(glm::vec3(1.0, 0.0, 0.0));
    m_colors.emplace_back(glm::vec3(1.0, 0.0, 0.0));

    m_indices.emplace_back(glm::uvec3(0,2,1));
    m_indices.emplace_back(glm::uvec3(1,2,3));

    m_indices.emplace_back(glm::uvec3(3,2,4));
    m_indices.emplace_back(glm::uvec3(3,4,5));

    m_indices.emplace_back(glm::uvec3(5,4,6));
    m_indices.emplace_back(glm::uvec3(5,6,7));

    std::vector<glm::vec3> m_normals {};
    m_normals.resize(m_positions.size(), glm::vec3(0.0f));
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

    std::cout << "Normals" << std::endl;
    for (const glm::vec3& vec : m_normals)
    {
        std::cout << vec[0] << "," << vec[1] << "," << vec[2] << "," << std::endl;
    }
}

void GeneratePyramid(
    std::vector<glm::vec3>& m_positions,
    std::vector<glm::vec3>& m_colors,
    std::vector<glm::uvec3>& m_indices
)
{
    m_positions.emplace_back(glm::vec3(0.0, 1.0, 0.0));
    m_positions.emplace_back(glm::vec3(-1.0, -1.0, 1.0));
    m_positions.emplace_back(glm::vec3(1.0, -1.0, 1.0));
    m_positions.emplace_back(glm::vec3(1.0, -1.0, -1.0));
    m_positions.emplace_back(glm::vec3(-1.0, -1.0, -1.0));

    m_colors.emplace_back(glm::vec3(1.0, 1.0, 1.0));
    m_colors.emplace_back(glm::vec3(1.0, 0.0, 0.0));
    m_colors.emplace_back(glm::vec3(0.0, 1.0, 0.0));
    m_colors.emplace_back(glm::vec3(0.0, 0.0, 1.0));
    m_colors.emplace_back(glm::vec3(1.0, 1.0, 0.0));

    m_indices.emplace_back(glm::uvec3(0,1,2)); // front
    m_indices.emplace_back(glm::uvec3(0,2,3)); // right
    m_indices.emplace_back(glm::uvec3(0,3,4)); // back
    m_indices.emplace_back(glm::uvec3(0,4,1)); // left

    m_indices.emplace_back(glm::uvec3(1,3,2));
    m_indices.emplace_back(glm::uvec3(1,4,3));
}

void GenerateSphere(
    std::vector<glm::vec3>& m_positions,
    std::vector<glm::vec3>& m_colors,
    std::vector<glm::uvec3>& m_indices,
    std::vector<glm::vec2>& m_tex_coords,
    int precision, float radius
)
{
    float degree_intervals{360.0f/(float)(precision-1)};
    int side_loop_count{precision/2};
    glm::vec3 debug_color {glm::vec3(1.0f, 1.0f, 1.0f)};

    //left to right x to -x

    //m_positions.emplace_back(glm::vec3(radius, 0.0f, 0.0f));
    //m_colors.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));
    //m_tex_coords.emplace_back(glm::vec2(0.5f, 1.0f));

    //left
    for (size_t i = 0; i <= side_loop_count; i++)
    {
        float theta{glm::radians((90.0f * (float)(i)) / (float)(side_loop_count + 1))};
        float ring_x{radius * std::cos(theta)};
        float ring_r{radius * std::sin(theta)};
        float v{(ring_x + radius) / (2.0f * radius)};
        for (size_t j = 0; j < precision; j++)
        {
            m_positions.emplace_back(glm::vec3(
                ring_x,
                ring_r * std::sin(glm::radians((float)j*degree_intervals)),
                ring_r * std::cos(glm::radians((float)j*degree_intervals))
            ));
            m_colors.emplace_back(glm::vec3(.33f, 0.33f, 0.33f));
            m_tex_coords.emplace_back(glm::vec2(((float)j*degree_intervals)/360.0f, v));
        }
    }

    //middle
    for (size_t j = 0; j < precision; j++)
    {
        m_positions.emplace_back(glm::vec3(
            0.0f,
            radius * std::sin(glm::radians((float)j*degree_intervals)),
            radius * std::cos(glm::radians((float)j*degree_intervals))
        ));
        m_colors.emplace_back(glm::vec3(0.5f, 0.5f, 0.5f));
        m_tex_coords.emplace_back(glm::vec2(((float)j*degree_intervals)/360.0f, 0.5f));
    }
    
    //right
    for (size_t i = 0; i <= side_loop_count; i++)
    {
        float theta{glm::radians((90.0f * (float)(side_loop_count - i)) / (float)(side_loop_count + 1))};
        float ring_x{-radius * std::cos(theta)};
        float ring_r{radius * std::sin(theta)};
        float v{(ring_x + radius) / (2.0f * radius)};
        for (size_t j = 0; j < precision; j++)
        {
            m_positions.emplace_back(glm::vec3(
                ring_x,
                ring_r * std::sin(glm::radians((float)j*degree_intervals)),
                ring_r * std::cos(glm::radians((float)j*degree_intervals))
            ));
            m_colors.emplace_back(glm::vec3(0.66f, 0.66f, 0.66f));
            m_tex_coords.emplace_back(glm::vec2(((float)j*degree_intervals)/360.0f, v));
        }
    }

    //m_positions.emplace_back(glm::vec3(-radius, 0.0f, 0.0f));
    //m_colors.emplace_back(glm::vec3(1.0f, 1.0f, 1.0f));
    //m_tex_coords.emplace_back(glm::vec2(0.5f, 0.0f));

    //left indices
    /*for (unsigned int i = 0; i < precision; i++)
    {
        m_indices.emplace_back(glm::uvec3(
            0,
            ((i + 1)%precision) + 1,
            (i%precision) + 1
        ));
    }*/
    //middle
    unsigned int final_base{};
    unsigned int ring_pairs{2 * (unsigned int)side_loop_count};
    ring_pairs += 2;
    for (unsigned i = 0; i < ring_pairs; i++)
    {
        unsigned curr_base{(i * precision) - 1};
        unsigned next_base{curr_base + (precision)};
        final_base = next_base;
        for (unsigned j = 0; j < precision; j++)
        {
            m_indices.emplace_back(glm::uvec3(
                curr_base + j + 1,
                (next_base + (j + 1)%precision) + 1,
                next_base + (j%precision) + 1
            ));
            m_indices.emplace_back(glm::uvec3(
                curr_base + j + 1,
                (curr_base + (j + 1)%precision) + 1,
                (next_base + (j + 1)%precision) + 1
            ));
        }
    }
    //right indices
    /*for (unsigned int i = 0; i < precision; i++)
    {
        m_indices.emplace_back(glm::uvec3(
            final_base + 1 + i,
            (final_base + (i + 1)%precision) + 1,
            (final_base + precision) + 1
        ));
    }*/

    /*
    for (const glm::vec2& vec : m_tex_coords)
    {
        std::cout << vec[0] << "," << vec[1] << std::endl;
    }
    
    std::cout << "Positions" << std::endl;
    int index{0};
    for (const glm::vec3& vec : m_positions)
    {
        std::cout << index << ": " << vec[0] << "," << vec[1] << "," << vec[2] << "," << std::endl;
        index++;
    }
    std::cout << "Indices" << std::endl;
    for (const glm::uvec3& vec : m_indices)
    {
        std::cout << vec[0] << "," << vec[1] << "," << vec[2] << "," << std::endl;
    }
    */
}

int main()
{
    int screen_width {1920}, screen_height {1080};
    Renderer renderer {screen_width, screen_height};

    std::cout << "Creating shader..." << std::endl;
    std::shared_ptr<Shader> shader_ptr {std::make_shared<Shader>("GameClient/src/Renderer/Shaders/shader.vs.glsl", "GameClient/src/Renderer/Shaders/shader.fs.glsl")};
    std::cout << "Shader created successfully." << std::endl;

    std::cout << "Creating shader..." << std::endl;
    std::shared_ptr<Shader> texture_shader_ptr {std::make_shared<Shader>("GameClient/src/Renderer/Shaders/texture.vs.glsl", "GameClient/src/Renderer/Shaders/texture.fs.glsl")};
    std::cout << "Shader created successfully." << std::endl;

    std::cout << "Creating shader..." << std::endl;
    std::shared_ptr<Shader> blinn_phong_shdader_ptr {std::make_shared<Shader>("GameClient/src/Renderer/Shaders/blinn_phong.vs.glsl", "GameClient/src/Renderer/Shaders/blinn_phong.fs.glsl")};
    std::cout << "Shader created successfully." << std::endl;

    std::vector<std::shared_ptr<Model>> models{};

    PointLight pointLight{};
    pointLight.AddShader(blinn_phong_shdader_ptr);

    std::vector<glm::vec3> m_positions {};
    std::vector<glm::vec3> m_colors {};
    std::vector<glm::uvec3> m_indices {};
    std::vector<glm::vec2> m_tex_coords {};

    float lenght{100.0f};
    float width{50.0f};
    float goal_lenght{20.0f};
    float side_border_lenght{75.0f};
    float min_size{5.0f};
    float diagnal_length{CalculateDiagonalLength(
        (width - goal_lenght)/2.0f,
        (lenght - side_border_lenght)/2.0f
    )};


    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, min_size, min_size, side_border_lenght);
    std::shared_ptr<Model> right_border_ptr {std::make_shared<Model>()};
    right_border_ptr->SetGeometry(m_positions, m_indices);
    right_border_ptr->SetMaterial("GameClient/assets/textures/tile.jpg", m_tex_coords);
    right_border_ptr->SetShader(blinn_phong_shdader_ptr);
    right_border_ptr->initializeForGL();
    right_border_ptr->Translate(glm::vec3(-1.0f * (width/2.0f + min_size/2.0f), 0, 0));
    right_border_ptr->UpdateModelMatrix();
    models.emplace_back(right_border_ptr);

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, min_size, min_size, side_border_lenght);
    std::shared_ptr<Model> left_border_ptr {std::make_shared<Model>()};
    left_border_ptr->SetGeometry(m_positions, m_indices);
    left_border_ptr->SetMaterial("GameClient/assets/textures/tile.jpg", m_tex_coords);
    left_border_ptr->SetShader(blinn_phong_shdader_ptr);
    left_border_ptr->initializeForGL();
    left_border_ptr->Translate(glm::vec3(1.0f * (width/2.0f + min_size/2.0f), 0, 0));
    left_border_ptr->UpdateModelMatrix();
    models.emplace_back(left_border_ptr);

    glm::vec3 translation;
    glm::vec3 rev_nor_translation;
    glm::vec3 final_translation;

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, diagnal_length, min_size, min_size);
    std::shared_ptr<Model> bottom_left_border_ptr {std::make_shared<Model>()};
    bottom_left_border_ptr->SetGeometry(m_positions, m_indices);
    bottom_left_border_ptr->SetMaterial("GameClient/assets/textures/tile.jpg", m_tex_coords);
    bottom_left_border_ptr->SetShader(blinn_phong_shdader_ptr);
    bottom_left_border_ptr->initializeForGL();
    bottom_left_border_ptr->RotateY(-39.806f);
    bottom_left_border_ptr->Translate(glm::vec3(goal_lenght/2.0f, 0, -(lenght/2.0f)));
    translation = glm::vec3((width - goal_lenght)/2.0f, 0.0f, (lenght - side_border_lenght)/2.0f);
    translation = translation/2.0f;
    rev_nor_translation = glm::normalize(glm::vec3(translation.z, translation.y, -translation.x));
    final_translation = translation + (rev_nor_translation * min_size/2.0f);
    bottom_left_border_ptr->Translate(final_translation);
    bottom_left_border_ptr->UpdateModelMatrix();
    models.emplace_back(bottom_left_border_ptr);

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, diagnal_length, min_size, min_size);
    std::shared_ptr<Model> bottom_right_border_ptr {std::make_shared<Model>()};
    bottom_right_border_ptr->SetGeometry(m_positions, m_indices);
    bottom_right_border_ptr->SetMaterial("GameClient/assets/textures/tile.jpg", m_tex_coords);
    bottom_right_border_ptr->SetShader(blinn_phong_shdader_ptr);
    bottom_right_border_ptr->initializeForGL();
    bottom_right_border_ptr->RotateY(39.806f);
    bottom_right_border_ptr->Translate(glm::vec3(-goal_lenght/2.0f, 0, -(lenght/2.0f)));
    translation = glm::vec3(-(width - goal_lenght)/2.0f, 0.0f, (lenght - side_border_lenght)/2.0f);
    translation = translation/2.0f;
    rev_nor_translation = glm::normalize(glm::vec3(translation.z, translation.y, -translation.x));
    final_translation = translation - (rev_nor_translation * min_size/2.0f);
    bottom_right_border_ptr->Translate(final_translation);
    bottom_right_border_ptr->UpdateModelMatrix();
    models.emplace_back(bottom_right_border_ptr);

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, goal_lenght, min_size, min_size);
    std::shared_ptr<Model> bottom_goal_border_ptr {std::make_shared<Model>()};
    bottom_goal_border_ptr->SetGeometry(m_positions, m_indices);
    bottom_goal_border_ptr->SetMaterial("GameClient/assets/textures/tile.jpg", m_tex_coords);
    bottom_goal_border_ptr->SetShader(blinn_phong_shdader_ptr);
    bottom_goal_border_ptr->initializeForGL();
    bottom_goal_border_ptr->Translate(glm::vec3(0, 0, -(lenght + min_size)/2.0f));
    bottom_goal_border_ptr->UpdateModelMatrix();
    models.emplace_back(bottom_goal_border_ptr);

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, diagnal_length, min_size, min_size);
    std::shared_ptr<Model> top_left_border_ptr {std::make_shared<Model>()};
    top_left_border_ptr->SetGeometry(m_positions, m_indices);
    top_left_border_ptr->SetMaterial("GameClient/assets/textures/tile.jpg", m_tex_coords);
    top_left_border_ptr->SetShader(blinn_phong_shdader_ptr);
    top_left_border_ptr->initializeForGL();
    top_left_border_ptr->RotateY(39.806f);
    top_left_border_ptr->Translate(glm::vec3(goal_lenght/2.0f, 0, (lenght/2.0f)));
    translation = glm::vec3((width - goal_lenght)/2.0f, 0.0f, -(lenght - side_border_lenght)/2.0f);
    translation = translation/2.0f;
    rev_nor_translation = glm::normalize(glm::vec3(translation.z, translation.y, -translation.x));
    final_translation = translation - (rev_nor_translation * min_size/2.0f);
    top_left_border_ptr->Translate(final_translation);
    top_left_border_ptr->UpdateModelMatrix();
    models.emplace_back(top_left_border_ptr);

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, diagnal_length, min_size, min_size);
    std::shared_ptr<Model> top_right_border_ptr {std::make_shared<Model>()};
    top_right_border_ptr->SetGeometry(m_positions, m_indices);
    top_right_border_ptr->SetMaterial("GameClient/assets/textures/tile.jpg", m_tex_coords);
    top_right_border_ptr->SetShader(blinn_phong_shdader_ptr);
    top_right_border_ptr->initializeForGL();
    top_right_border_ptr->RotateY(-39.806f);
    top_right_border_ptr->Translate(glm::vec3(-goal_lenght/2.0f, 0, (lenght/2.0f)));
    translation = glm::vec3(-(width - goal_lenght)/2.0f, 0.0f, -(lenght - side_border_lenght)/2.0f);
    translation = translation/2.0f;
    rev_nor_translation = glm::normalize(glm::vec3(translation.z, translation.y, -translation.x));
    final_translation = translation + (rev_nor_translation * min_size/2.0f);
    top_right_border_ptr->Translate(final_translation);
    top_right_border_ptr->UpdateModelMatrix();
    models.emplace_back(top_right_border_ptr);

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateTexturedRectanle(m_positions, m_tex_coords, m_indices, goal_lenght, min_size, min_size);
    std::shared_ptr<Model> top_goal_border_ptr {std::make_shared<Model>()};
    top_goal_border_ptr->SetGeometry(m_positions, m_indices);
    top_goal_border_ptr->SetMaterial("GameClient/assets/textures/tile.jpg", m_tex_coords);
    top_goal_border_ptr->SetShader(blinn_phong_shdader_ptr);
    top_goal_border_ptr->initializeForGL();
    top_goal_border_ptr->Translate(glm::vec3(0, 0, (lenght + min_size)/2.0f));
    top_goal_border_ptr->UpdateModelMatrix();
    models.emplace_back(top_goal_border_ptr);

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateXZBase(m_positions, m_colors, m_indices, m_tex_coords, width, lenght, goal_lenght, side_border_lenght);
    std::shared_ptr<Model> floor_ptr {std::make_shared<Model>()};
    floor_ptr->SetGeometry(m_positions, m_indices);
    floor_ptr->SetMaterial("GameClient/assets/textures/base.png", m_tex_coords);
    floor_ptr->SetShader(blinn_phong_shdader_ptr);
    floor_ptr->initializeForGL();
    floor_ptr->Translate(glm::vec3(0, -5, 0));
    floor_ptr->UpdateModelMatrix();
    models.emplace_back(floor_ptr);
    
    std::shared_ptr<Model> handle_ptr {
        ModelLoader::LoadFromFile("GameClient/assets/models/handle.obj", blinn_phong_shdader_ptr)
    };
    if (handle_ptr == nullptr)
    {
        std::cout << "Failed to load GameClient/assets/models/handle.obj" << std::endl;
    }
    else
    {
        handle_ptr->Translate(glm::vec3(-25.0f, 0, -50.0f)); // TODO: adjust to where it should actually sit
        handle_ptr->ScaleDimensionToMaxSize(10.0f, 1);
        handle_ptr->RotateX(-90.0f);
        handle_ptr->UpdateModelMatrix();
        models.emplace_back(handle_ptr);
    }

    std::cout << "Generating Sphere" << std::endl;
    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    m_tex_coords.clear();
    GenerateSphere(m_positions, m_colors, m_indices, m_tex_coords, 30, 5);
    std::shared_ptr<Model> sphere_ptr {std::make_shared<Model>()};
    sphere_ptr->SetGeometry(m_positions, m_indices, true);
    sphere_ptr->SetMaterial("GameClient/assets/textures/ball.png", m_tex_coords);
    sphere_ptr->SetShader(blinn_phong_shdader_ptr);
    sphere_ptr->initializeForGL();
    sphere_ptr->Translate(glm::vec3(0, 0, 0));
    sphere_ptr->UpdateModelMatrix();
    models.emplace_back(sphere_ptr);

    float time{};
    while (!renderer.shouldClose())
    {
        renderer.clear();
        
        blinn_phong_shdader_ptr->use();
        blinn_phong_shdader_ptr->setMat4("view", renderer.camera.view);
        blinn_phong_shdader_ptr->setMat4("projection", renderer.camera.proj);
        pointLight.PassUniforms();
        
        for (const std::shared_ptr<Model>& model_ptr : models)
        {
            model_ptr->draw();
        }

        renderer.render();
    }
    
    return 0;
}