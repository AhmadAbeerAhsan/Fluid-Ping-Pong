#include "Boundary.hpp"

Boundary::Boundary(float user_speed_per_second, float mass) :
    m_user_speed_per_second(user_speed_per_second),
    m_mass(mass),
    m_reflective_normal_boundaries{}
{
    start = std::chrono::steady_clock::now();
}

Boundary::~Boundary()
{
}

void Boundary::SetUserVelocity(glm::vec2 normalized_dir)
{
    m_boundary_velocity = m_user_speed_per_second * normalized_dir;
}

void Boundary::SetVelocity(glm::vec2 velocity)
{
    m_boundary_velocity = velocity;
}

void Boundary::SetUserSpeedPerSecond(float speed)
{
    m_user_speed_per_second = speed;
}

void Boundary::AssignModel(std::shared_ptr<Model> model)
{
    m_model = model;
    m_boundary_positions.x = m_model->m_model_positions.x;
    m_boundary_positions.y = m_model->m_model_positions.z;
}

void Boundary::Move()
{
    float deltaTime = ImGui::GetIO().DeltaTime;
    m_boundary_positions.x += deltaTime * m_boundary_velocity.x;
    m_boundary_positions.y += deltaTime * m_boundary_velocity.y;
    glm::vec2 model_pos{m_model->m_model_positions.x, m_model->m_model_positions.z};
    glm::vec2 displacement = model_pos - m_boundary_positions;
    now = std::chrono::steady_clock::now();
    int delta_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
    if (glm::length(displacement) < 1.0f || (delta_ms > m_threshold_ms))
    {
        m_model->m_model_positions.x = m_boundary_positions.x;
        m_model->m_model_positions.z = m_boundary_positions.y;
    }
    else
    {
        m_model->m_model_positions.x = (m_model->m_model_positions.x * (float)(m_threshold_ms - delta_ms)/float(m_threshold_ms)) + (m_boundary_positions.x * (float)delta_ms/float(m_threshold_ms));
        m_model->m_model_positions.z = (m_model->m_model_positions.z * (float)(m_threshold_ms - delta_ms)/float(m_threshold_ms)) + (m_boundary_positions.y * (float)delta_ms/float(m_threshold_ms));
    }
}

void Boundary::Update()
{
    m_model->UpdateModelMatrix();
}

void Boundary::SetPosition(glm::vec2 pos, int new_threshold_ms)
{
    //m_model->m_model_positions.x = m_boundary_positions.x;
    //m_model->m_model_positions.z = m_boundary_positions.y;

    m_boundary_positions = pos;

    start = std::chrono::steady_clock::now();
    m_threshold_ms = new_threshold_ms;
}

void Boundary::Tranaslate(glm::vec2 vec)
{
    m_boundary_positions.x += vec.x;
    m_boundary_positions.y += vec.y;
}

void Boundary::Reflect(glm::vec2 normal)
{
    m_boundary_velocity = glm::reflect(m_boundary_velocity, normal);
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
    return m_boundary_positions;
}
