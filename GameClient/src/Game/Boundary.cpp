#include "Boundary.hpp"

Boundary::Boundary()
{
}

Boundary::~Boundary()
{
}

void Boundary::SetDirection(glm::vec2 dir)
{
    m_dir = dir;
}

void Boundary::AssignModel(std::shared_ptr<Model> model)
{
    m_model = model;
}

void Boundary::Move()
{
    m_model->m_model_positions.x += m_speed_per_second * m_dir.x;
    m_model->m_model_positions.z += m_speed_per_second * m_dir.y;
}
