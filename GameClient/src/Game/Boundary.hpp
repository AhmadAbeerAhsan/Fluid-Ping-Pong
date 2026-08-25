#pragma once

#include <memory>
#include <vector>
    
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Renderer/Model.hpp"
#include "../UI/UI.hpp"

class Boundary
{
protected:
    glm::vec2 m_velocity{0.0f, 0.0f};
    float m_user_speed_per_second;
    float m_mass;
    std::shared_ptr<Model> m_model;
    std::vector<glm::vec2> m_reflective_normal_boundaries;
    
public:
    Boundary(float user_speed_per_second, float mass);
    virtual ~Boundary();

    virtual void SetUserVelocity(glm::vec2 normalized_dir);
    virtual void SetVelocity(glm::vec2 velocity);
    virtual void SetUserSpeedPerSecond(float speed);
    virtual void AssignModel(std::shared_ptr<Model> model);
    virtual void Move();
    virtual void Update();
    virtual void SetPosition(glm::vec2 pos);
    virtual void Reflect(glm::vec2 normal);
    virtual void ClearReflection();
    virtual bool TryTranaslate(glm::vec2 vec);
    virtual void Tranaslate(glm::vec2 vec);

    virtual glm::vec2 Origin();
    virtual glm::vec2 Velocity(){ return m_velocity; }
    virtual float Mass(){ return m_mass; }

    virtual bool CollideAgainstLine(std::shared_ptr<Boundary> boundary_ptr) = 0;
    virtual bool CollideAgainstCircle(std::shared_ptr<Boundary> boundary_ptr) = 0;
};

