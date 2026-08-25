#pragma once

#include "Boundary.hpp"

class BoundaryCircle : public Boundary
{
private:
    float m_radius;
public:
    BoundaryCircle(float radius, float user_speed_per_second, float mass);
    ~BoundaryCircle() override = default;

    float Radius(){ return m_radius; }

    bool CollideAgainstLine(std::shared_ptr<Boundary> boundary_ptr) override;
    bool CollideAgainstCircle(std::shared_ptr<Boundary> boundary_ptr) override;
};