#pragma once

#include <memory>
    
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Renderer/Model.hpp"

class Boundary
{
public:
    enum BoundayType
    {
        Circular,
        X_Line,
        Z_Line,
        XZ_Line
    };

private:
    glm::vec2 m_dir{0.0f, 0.0f};
    float m_speed_per_second{0.0f};

    std::shared_ptr<Model> m_model;
    
public:
    Boundary(/* args */);
    ~Boundary();

    void SetDirection(glm::vec2 dir);
    void AssignModel(std::shared_ptr<Model> model);
    void Move();
};

