#pragma once

#include <memory>
#include <vector>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Renderer/Model.hpp"
#include "../UI/UI.hpp"
#include <algorithm>

constexpr int player_interpolation_duration_short_ms{400};
constexpr int ball_interpolation_duration_short_ms{400};
constexpr int player_interpolation_duration_long_ms{1500};
constexpr int ball_interpolation_duration_long_ms{1500};

class Boundary
{
protected:
    glm::vec2 m_boundary_velocity{0.0f, 0.0f}; //used for collisions, model positions are used for display only, adjust in move function*
    glm::vec2 m_boundary_positions{0.0f, 0.0f}; //used for collisions, model positions are used for display only, adjust in move function*
    float m_user_speed_per_second;
    float m_mass;
    std::shared_ptr<Model> m_model;
    std::vector<glm::vec2> m_reflective_normal_boundaries;
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point now;
    int m_threshold_ms{1000};

public:
    Boundary(float user_speed_per_second, float mass);
    virtual ~Boundary();

    virtual void SetUserVelocity(glm::vec2 normalized_dir);
    virtual void SetVelocity(glm::vec2 velocity);
    virtual void SetUserSpeedPerSecond(float speed);
    virtual float UserSpeedPerSecond(){ return m_user_speed_per_second; }
    virtual void AssignModel(std::shared_ptr<Model> model);
    virtual void Move();
    virtual void Update();
    virtual void SetPosition(glm::vec2 pos, int new_threshold_ms);
    virtual void Reflect(glm::vec2 normal);
    virtual void ClearReflection();
    virtual bool TryTranaslate(glm::vec2 vec);
    virtual void Tranaslate(glm::vec2 vec);

    virtual glm::vec2 Origin();
    virtual glm::vec2 Velocity(){ return m_boundary_velocity; }
    virtual float Mass(){ return m_mass; }
    virtual void Mass(float m) { m_mass = m; }

    virtual bool CollideAgainstLine(std::shared_ptr<Boundary> boundary_ptr) = 0;
    virtual bool CollideAgainstCircle(std::shared_ptr<Boundary> boundary_ptr) = 0;
};

