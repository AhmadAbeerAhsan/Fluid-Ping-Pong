#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
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

    /*
    std::cout << "Normals" << std::endl;
    for (const glm::vec3& vec : m_normals)
    {
        std::cout << vec[0] << "," << vec[1] << "," << vec[2] << "," << std::endl;
    }
    */
}

void GenerateXZBase2(
    std::vector<glm::vec3>& m_positions,
    std::vector<glm::vec3>& m_colors,
    std::vector<glm::uvec3>& m_indices,
    std::vector<glm::vec2>& m_tex_coords,
    float x_size,
    float z_size
)
{
    // Rectangle corners on the XZ plane
    m_positions.emplace_back( x_size / 2.0f, 0.0f, -z_size / 2.0f); // 0: bottom-left
    m_positions.emplace_back( -x_size / 2.0f, 0.0f, -z_size / 2.0f); // 1: bottom-right
    m_positions.emplace_back( -x_size / 2.0f, 0.0f,  z_size / 2.0f); // 2: top-right
    m_positions.emplace_back(x_size / 2.0f, 0.0f,  z_size / 2.0f); // 3: top-left

    // Use the entire texture
    m_tex_coords.emplace_back(0.0f, 0.0f); // 0
    m_tex_coords.emplace_back(1.0f, 0.0f); // 1
    m_tex_coords.emplace_back(1.0f, 1.0f); // 2
    m_tex_coords.emplace_back(0.0f, 1.0f); // 3

    // Color
    for (int i = 0; i < 4; ++i)
    {
        m_colors.emplace_back(1.0f, 0.0f, 0.0f);
    }

    // Two triangles
    m_indices.emplace_back(0, 2, 3);
    m_indices.emplace_back(0, 1, 2);
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

void GenerateSkyboxCube(
    std::vector<glm::vec3>& m_positions,
    std::vector<glm::vec2>& m_tex_coords,
    std::vector<glm::uvec3>& m_indices
)
{
    m_positions = {
        // back face
        {-1.0f,  1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f, -1.0f},
        { 1.0f, -1.0f, -1.0f}, { 1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f},

        // right face
        { 1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f,  1.0f}, { 1.0f,  1.0f, -1.0f},
        { 1.0f, -1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}, { 1.0f,  1.0f, -1.0f},

        // front face
        { 1.0f, -1.0f,  1.0f}, {-1.0f, -1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f},
        {-1.0f, -1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f},

        // left face
        {-1.0f, -1.0f,  1.0f}, {-1.0f, -1.0f, -1.0f}, {-1.0f,  1.0f,  1.0f},
        {-1.0f, -1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f,  1.0f},

        // bottom face
        {-1.0f, -1.0f,  1.0f}, { 1.0f, -1.0f,  1.0f}, { 1.0f, -1.0f, -1.0f},
        { 1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f,  1.0f},

        // top face
        {-1.0f,  1.0f, -1.0f}, { 1.0f,  1.0f, -1.0f}, { 1.0f,  1.0f,  1.0f},
        { 1.0f,  1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f}, {-1.0f,  1.0f, -1.0f}
    };

    m_tex_coords = {
        // back face
        {1.00f, 0.6666666f}, {1.00f, 0.3333333f}, {0.75f, 0.3333333f},
        {0.75f, 0.3333333f}, {0.75f, 0.6666666f}, {1.00f, 0.6666666f},

        // right face
        {0.75f, 0.3333333f}, {0.50f, 0.3333333f}, {0.75f, 0.6666666f},
        {0.50f, 0.3333333f}, {0.50f, 0.6666666f}, {0.75f, 0.6666666f},

        // front face
        {0.50f, 0.3333333f}, {0.25f, 0.3333333f}, {0.50f, 0.6666666f},
        {0.25f, 0.3333333f}, {0.25f, 0.6666666f}, {0.50f, 0.6666666f},

        // left face
        {0.25f, 0.3333333f}, {0.00f, 0.3333333f}, {0.25f, 0.6666666f},
        {0.00f, 0.3333333f}, {0.00f, 0.6666666f}, {0.25f, 0.6666666f},

        // bottom face
        {0.25f, 0.3333333f}, {0.50f, 0.3333333f}, {0.50f, 0.0000000f},
        {0.50f, 0.0000000f}, {0.25f, 0.0000000f}, {0.25f, 0.3333333f},

        // top face
        {0.25f, 1.0000000f}, {0.50f, 1.0000000f}, {0.50f, 0.6666666f},
        {0.50f, 0.6666666f}, {0.25f, 0.6666666f}, {0.25f, 1.0000000f}
    };

    // No sharing between triangles, so indices are just a trivial
    // sequential grouping of the array above into (0,1,2), (3,4,5), ...
    m_indices.reserve(m_positions.size() / 3);
    for (unsigned int i = 0; i < m_positions.size(); i += 3)
    {
        m_indices.emplace_back(i, i + 1, i + 2);
    }
}

void GenerateFullscreenQuad(
    std::vector<glm::vec3>& m_positions,
    std::vector<glm::vec2>& m_tex_coords,
    std::vector<glm::uvec3>& m_indices
)
{
    m_positions = {
        {-1.0f,  1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, { 1.0f, -1.0f, 0.0f},
        {-1.0f,  1.0f, 0.0f}, { 1.0f, -1.0f, 0.0f}, { 1.0f,  1.0f, 0.0f}
    };

    m_tex_coords = {
        {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
        {0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}
    };

    // No sharing between triangles, so indices are just a trivial
    // sequential grouping of the array above into (0,1,2), (3,4,5).
    m_indices.reserve(m_positions.size() / 3);
    for (unsigned int i = 0; i < m_positions.size(); i += 3)
    {
        m_indices.emplace_back(i, i + 1, i + 2);
    }
}

void GenerateHandle(
    std::vector<glm::vec3>& m_positions,
    std::vector<glm::vec3>& m_colors,
    std::vector<glm::uvec3>& m_indices,
    std::vector<glm::vec2>& m_tex_coords,
    float r1,
    float h1, float h2, float h3,
    float b1, float b2,
    int precision,
    glm::vec3 primary_color,
    glm::vec3 secondary_color
)
{
    int top_arc_points = precision;
    int total_rotations = precision * 5;

    float delta_arc_angle{
        glm::radians(
            90.0f/(float)(precision - 1)
        )
    };
    float delta_rotaion_angle{
        glm::radians(
            360.0f/(float)total_rotations
        )
    };

    int first_loop_size{0};
    //top arc
    for (size_t i = 0; i < precision; i++)
    {
        float delta{(float)i * delta_arc_angle};
        m_positions.emplace_back(r1*std::sin(delta), r1*std::cos(delta) + h1 + h2, 0.0f);
        m_tex_coords.emplace_back(0.5f, 0.5f);
        m_colors.emplace_back(secondary_color);
        first_loop_size++;  
    }
    //arc end till arc end + partial arc end
    //first_loop_size++;
    //m_positions.emplace_back(r1, h2 + h1 - 0.2f, 0.0f);
    //m_tex_coords.emplace_back(0.5f, 0.5f);
    //m_colors.emplace_back(secondary_color);
    //arc end till base start
    first_loop_size++;
    m_positions.emplace_back(r1, h2, 0.0f);
    m_tex_coords.emplace_back(0.5f, 0.5f);
    m_colors.emplace_back(secondary_color);
    //base start till base bottom outer boundary inner side halfway
    first_loop_size++;
    m_positions.emplace_back(r1 + b1/2.0f, h2, 0.0f);
    m_tex_coords.emplace_back(0.5f, 0.5f);
    m_colors.emplace_back(secondary_color);
    //base bottom outer boundary inner side halfway till base bottom outer boundary inner side
    first_loop_size++;
    m_positions.emplace_back(r1 + b1, h2, 0.0f);
    m_tex_coords.emplace_back(0.5f, 0.5f);
    m_colors.emplace_back(primary_color);
    //base bottom outer boundary inner side till base top outer boundary inner side
    first_loop_size++;
    m_positions.emplace_back(r1 + b1, h2 + h3, 0.0f);
    m_tex_coords.emplace_back(0.5f, 0.5f);
    m_colors.emplace_back(primary_color);
    //base top outer boundary inner side till base top outer boundary outer side
    first_loop_size++;
    m_positions.emplace_back(r1 + b1 + b2, h2 + h3, 0.0f);
    m_tex_coords.emplace_back(0.5f, 0.5f);
    m_colors.emplace_back(primary_color);
    //base top outer boundary outer side till base bottom outer boundary outer side
    first_loop_size++;
    m_positions.emplace_back(r1 + b1 + b2, 0.0f, 0.0f);
    m_tex_coords.emplace_back(0.5f, 0.5f);
    m_colors.emplace_back(primary_color);
    //base bottom outer boundary outer side till base bottom center
    first_loop_size++;
    m_positions.emplace_back(0.0f, 0.0f, 0.0f);
    m_tex_coords.emplace_back(0.5f, 0.5f);
    m_colors.emplace_back(primary_color);

    for (size_t i = 1; i <= total_rotations; i++)
    {
        float delta{(float)i * delta_rotaion_angle};
        for (size_t j = 0; j < first_loop_size; j++)
        {
            glm::vec3 p = m_positions[j];

            float x = p.x;
            float y = p.y;

            m_positions.emplace_back(
                x * std::cos(delta),
                y,
                x * std::sin(delta)
            );
            m_tex_coords.emplace_back(0.5f, 0.5f);
            m_colors.emplace_back(m_colors[j]);
        }
    }

    unsigned int loop_pairs{(unsigned int)total_rotations};
    for (unsigned i = 0; i < loop_pairs; i++)
    {
        unsigned curr_base{i * first_loop_size};
        unsigned next_base{curr_base + (first_loop_size)};
        for (unsigned j = 0; j < first_loop_size - 1; j++)
        {
            m_indices.emplace_back(glm::uvec3(
                curr_base + j,
                next_base + (j%first_loop_size),
                (next_base + (j + 1)%first_loop_size)
            ));
            m_indices.emplace_back(glm::uvec3(
                curr_base + j,
                (next_base + (j + 1)%first_loop_size),
                (curr_base + (j + 1)%first_loop_size)
            ));
        }
    }
}