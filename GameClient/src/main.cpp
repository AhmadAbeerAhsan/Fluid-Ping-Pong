#include "Renderer/Renderer.hpp"
#include "Renderer/Model.hpp"
#include "Renderer/Shader.hpp"

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
    m_positions.emplace_back(glm::vec3(-1.0 * x_size, -1.0 * y_size, 1.0 * z_size));
    m_positions.emplace_back(glm::vec3(1.0 * x_size, -1.0 * y_size, 1.0 * z_size));
    m_positions.emplace_back(glm::vec3(-1.0 * x_size, 1.0 * y_size, 1.0 * z_size));
    m_positions.emplace_back(glm::vec3(1.0 * x_size, 1.0 * y_size, 1.0 * z_size));
    m_positions.emplace_back(glm::vec3(-1.0 * x_size, -1.0 * y_size, -1.0 * z_size));
    m_positions.emplace_back(glm::vec3(1.0 * x_size, -1.0 * y_size, -1.0 * z_size));
    m_positions.emplace_back(glm::vec3(-1.0 * x_size, 1.0 * y_size, -1.0 * z_size));
    m_positions.emplace_back(glm::vec3(1.0 * x_size, 1.0 * y_size, -1.0 * z_size));

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

void GenerateXZPlane(
    std::vector<glm::vec3>& m_positions,
    std::vector<glm::vec3>& m_colors,
    std::vector<glm::uvec3>& m_indices,
    float x_size, float z_size
)
{
    m_positions.emplace_back(glm::vec3(-1.0 * x_size, 0, -1.0 * z_size));
    m_positions.emplace_back(glm::vec3(1.0 * x_size, 0, -1.0 * z_size));
    m_positions.emplace_back(glm::vec3(-1.0 * x_size, 0, 1.0 * z_size));
    m_positions.emplace_back(glm::vec3(1.0 * x_size, 0, 1.0 * z_size));

    m_colors.emplace_back(glm::vec3(0.0, 0.0, 0.0));
    m_colors.emplace_back(glm::vec3(1.0, 0.0, 0.0));
    m_colors.emplace_back(glm::vec3(0.0, 1.0, 1.0));
    m_colors.emplace_back(glm::vec3(1.0, 1.0, 1.0));

    m_indices.emplace_back(glm::uvec3(0,2,3));
    m_indices.emplace_back(glm::uvec3(1,0,3));
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

int main()
{
    int screen_width {1920}, screen_height {1080};
    Renderer renderer {screen_width, screen_height};

    std::cout << "Creating shader..." << std::endl;
    std::shared_ptr<Shader> shader_ptr {std::make_shared<Shader>("GameClient/src/Renderer/Shaders/shader.vs.glsl", "GameClient/src/Renderer/Shaders/shader.fs.glsl")};
    std::cout << "Shader created successfully." << std::endl;

    std::vector<glm::vec3> m_positions {};
    std::vector<glm::vec3> m_colors {};
    std::vector<glm::uvec3> m_indices {};

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    GenerateRectanle(m_positions, m_colors, m_indices, 5, 5, 110);
    std::shared_ptr<Model> right_border_ptr {std::make_shared<Model>()};
    right_border_ptr->SetGeometry(m_positions, m_colors, m_indices);
    right_border_ptr->SetMaterial(shader_ptr);
    right_border_ptr->initializeForGL();
    right_border_ptr->Translate(glm::vec3(-55, 0, 0));

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    GenerateRectanle(m_positions, m_colors, m_indices, 5, 5, 110);
    std::shared_ptr<Model> left_border_ptr {std::make_shared<Model>()};
    left_border_ptr->SetGeometry(m_positions, m_colors, m_indices);
    left_border_ptr->SetMaterial(shader_ptr);
    left_border_ptr->initializeForGL();
    left_border_ptr->Translate(glm::vec3(55, 0, 0));

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    GenerateRectanle(m_positions, m_colors, m_indices, 17.5f, 5, 5);
    std::shared_ptr<Model> bottom_left_border_ptr {std::make_shared<Model>()};
    bottom_left_border_ptr->SetGeometry(m_positions, m_colors, m_indices);
    bottom_left_border_ptr->SetMaterial(shader_ptr);
    bottom_left_border_ptr->initializeForGL();
    bottom_left_border_ptr->Translate(glm::vec3(15.0f + 17.5f, 0, -105));

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    GenerateRectanle(m_positions, m_colors, m_indices, 17.5f, 5, 5);
    std::shared_ptr<Model> bottom_right_border_ptr {std::make_shared<Model>()};
    bottom_right_border_ptr->SetGeometry(m_positions, m_colors, m_indices);
    bottom_right_border_ptr->SetMaterial(shader_ptr);
    bottom_right_border_ptr->initializeForGL();
    bottom_right_border_ptr->Translate(glm::vec3(-15.0f - 17.5f, 0, -105));

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    GenerateRectanle(m_positions, m_colors, m_indices, 15, 5, 5);
    std::shared_ptr<Model> bottom_goal_border_ptr {std::make_shared<Model>()};
    bottom_goal_border_ptr->SetGeometry(m_positions, m_colors, m_indices);
    bottom_goal_border_ptr->SetMaterial(shader_ptr);
    bottom_goal_border_ptr->initializeForGL();
    bottom_goal_border_ptr->Translate(glm::vec3(0, 0, -105));

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    GenerateRectanle(m_positions, m_colors, m_indices, 17.5f, 5, 5);
    std::shared_ptr<Model> top_left_border_ptr {std::make_shared<Model>()};
    top_left_border_ptr->SetGeometry(m_positions, m_colors, m_indices);
    top_left_border_ptr->SetMaterial(shader_ptr);
    top_left_border_ptr->initializeForGL();
    top_left_border_ptr->Translate(glm::vec3(15.0f + 17.5f, 0, 105));

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    GenerateRectanle(m_positions, m_colors, m_indices, 17.5f, 5, 5);
    std::shared_ptr<Model> top_right_border_ptr {std::make_shared<Model>()};
    top_right_border_ptr->SetGeometry(m_positions, m_colors, m_indices);
    top_right_border_ptr->SetMaterial(shader_ptr);
    top_right_border_ptr->initializeForGL();
    top_right_border_ptr->Translate(glm::vec3(-15.0f - 17.5f, 0, 105));

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    GenerateRectanle(m_positions, m_colors, m_indices, 15, 5, 5);
    std::shared_ptr<Model> top_goal_border_ptr {std::make_shared<Model>()};
    top_goal_border_ptr->SetGeometry(m_positions, m_colors, m_indices);
    top_goal_border_ptr->SetMaterial(shader_ptr);
    top_goal_border_ptr->initializeForGL();
    top_goal_border_ptr->Translate(glm::vec3(0, 0, 105));

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    GenerateXZPlane(m_positions, m_colors, m_indices, 50, 100);
    std::shared_ptr<Model> floor_ptr {std::make_shared<Model>()};
    floor_ptr->SetGeometry(m_positions, m_colors, m_indices);
    floor_ptr->SetMaterial(shader_ptr);
    floor_ptr->initializeForGL();
    floor_ptr->Translate(glm::vec3(0, -5, 0));

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    GenerateRectanle(m_positions, m_colors, m_indices, 10, 5, 5);
    std::shared_ptr<Model> bottom_player_ptr {std::make_shared<Model>()};
    bottom_player_ptr->SetGeometry(m_positions, m_colors, m_indices);
    bottom_player_ptr->SetMaterial(shader_ptr);
    bottom_player_ptr->initializeForGL();
    bottom_player_ptr->Translate(glm::vec3(0, 0, -95));

    m_positions.clear();
    m_colors.clear();
    m_indices.clear();
    GenerateRectanle(m_positions, m_colors, m_indices, 10, 5, 5);
    std::shared_ptr<Model> top_player_ptr {std::make_shared<Model>()};
    top_player_ptr->SetGeometry(m_positions, m_colors, m_indices);
    top_player_ptr->SetMaterial(shader_ptr);
    top_player_ptr->initializeForGL();
    top_player_ptr->Translate(glm::vec3(0, 0, 95));

    float time{};
    while (!renderer.shouldClose())
    {
        renderer.clear();

        shader_ptr->use();
        //shader_ptr->setFloat("time", renderer.timer.currentTime / 1.0f);
        shader_ptr->setMat4("view", renderer.camera.view);
        shader_ptr->setMat4("projection", renderer.camera.proj);

        right_border_ptr->draw();     
        left_border_ptr->draw();
        bottom_left_border_ptr->draw();
        bottom_right_border_ptr->draw();
        bottom_goal_border_ptr->draw();
        top_left_border_ptr->draw();
        top_right_border_ptr->draw();
        top_goal_border_ptr->draw(); 
        floor_ptr->draw(); 
        bottom_player_ptr->draw(); 
        top_player_ptr->draw(); 


        renderer.render();
    }
    
    return 0;
}