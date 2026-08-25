#include "BoundaryLine.hpp"

float BoundaryLine::EdgeFunction(glm::vec2 p)
{
    std::cout << "BoundaryLine::EdgeFunction" << std::endl;
    return (p.x - m_pxz.x)*m_dxz.y - (p.y - m_pxz.y)*m_dxz.x;
}

BoundaryLine::BoundaryLine(glm::vec2 pxz, glm::vec2 dxz) : Boundary(0.0f, 0.0f),
                                                           m_pxz(pxz),
                                                           m_dxz(glm::normalize(dxz))
{
    m_normal = glm::vec2(-m_dxz.y, m_dxz.x);
}

bool BoundaryLine::CollideAgainstLine(std::shared_ptr<Boundary> boundary_ptr)
{
    return false;
}

bool BoundaryLine::CollideAgainstCircle(std::shared_ptr<Boundary> boundary_ptr)
{
    auto other_circle = std::dynamic_pointer_cast<BoundaryCircle>(boundary_ptr);
    if (!other_circle)
    {
        std::cout << "BoundaryCircle::CollideAgainstCircle: other_circle" << std::endl;
        return false;
    }
    float dot {glm::dot(m_normal, other_circle->Velocity())};
    if (dot < 0.0f)
    {
        std::cout << "BoundaryCircle::CollideAgainstCircle: dot < 0.0f" << std::endl;
        float sign = EdgeFunction(other_circle->Origin()) + other_circle->Radius();
        return sign > 0.0f;
    }
    
    return false;
}
