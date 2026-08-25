#include "Boundary.hpp"

Boundary::Boundary(float user_speed_per_second, float mass) :
    m_user_speed_per_second(user_speed_per_second),
    m_mass(mass),
    m_reflective_normal_boundaries{}
{
}

Boundary::~Boundary()
{
}

void Boundary::SetUserVelocity(glm::vec2 normalized_dir)
{
    m_velocity = m_user_speed_per_second * normalized_dir;
}

void Boundary::SetVelocity(glm::vec2 velocity)
{
    m_velocity = velocity;
}

void Boundary::SetUserSpeedPerSecond(float speed)
{
    m_user_speed_per_second = speed;
}

void Boundary::AssignModel(std::shared_ptr<Model> model)
{
    m_model = model;
}

void Boundary::Move()
{
    float deltaTime = ImGui::GetIO().DeltaTime;
    m_model->m_model_positions.x += deltaTime * m_velocity.x;
    m_model->m_model_positions.z += deltaTime * m_velocity.y;
}

void Boundary::Update()
{
    m_model->UpdateModelMatrix();
}

void Boundary::SetPosition(glm::vec2 pos)
{
    m_model->m_model_positions.x = pos.x;
    m_model->m_model_positions.z = pos.y;
}

void Boundary::Tranaslate(glm::vec2 vec)
{
    m_model->m_model_positions.x += vec.x;
    m_model->m_model_positions.z += vec.y;
}

void Boundary::Reflect(glm::vec2 normal)
{
    m_velocity = glm::reflect(m_velocity, normal);
    m_reflective_normal_boundaries.emplace_back(normal);
}

void Boundary::ClearReflection()
{
    m_reflective_normal_boundaries.clear();
}

bool Boundary::TryTranaslate(glm::vec2 vec)
{
    for ( glm::vec2& normal : m_reflective_normal_boundaries)
    {
        if (glm::dot(vec, normal) < 0.0f)
        {
            return false;
        }
    }
    Tranaslate(vec);
    return true;
}

glm::vec2 Boundary::Origin()
{
    return glm::vec2(m_model->m_model_positions.x, m_model->m_model_positions.z);
}
