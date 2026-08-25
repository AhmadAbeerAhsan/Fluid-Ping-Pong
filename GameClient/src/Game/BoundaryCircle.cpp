#include "BoundaryCircle.hpp"

BoundaryCircle::BoundaryCircle(float radius, float user_speed_per_second, float mass) :
    Boundary(user_speed_per_second, mass),
    m_radius(radius)
{
}

bool BoundaryCircle::CollideAgainstLine(std::shared_ptr<Boundary> boundary_ptr)
{
    return false;
}

bool BoundaryCircle::CollideAgainstCircle(std::shared_ptr<Boundary> boundary_ptr)
{
    auto other_circle = std::dynamic_pointer_cast<BoundaryCircle>(boundary_ptr);
    if (!other_circle)
    {
        std::cout << "BoundaryCircle::CollideAgainstCircle: other_circle" << std::endl;
        return false;
    }
    
    glm::vec2 distance{Origin() - other_circle->Origin()};
    float sqr_distance{glm::dot(distance, distance)};
    float sum_radius{m_radius + other_circle->m_radius};
    return sqr_distance <= (sum_radius * sum_radius);
}
