#pragma once

#include "BoundaryCircle.hpp"

class BoundaryLine : public Boundary
{
private:
    glm::vec2 m_pxz;
    glm::vec2 m_dxz;
    glm::vec2 m_normal;

    float EdgeFunction(glm::vec2 p);
public:
    BoundaryLine(glm::vec2 pxz, glm::vec2 dxz);
    ~BoundaryLine() override = default;

    glm::vec2 Normal(){ return m_normal; }

    bool CollideAgainstLine(std::shared_ptr<Boundary> boundary_ptr) override;
    bool CollideAgainstCircle(std::shared_ptr<Boundary> boundary_ptr) override;
};

