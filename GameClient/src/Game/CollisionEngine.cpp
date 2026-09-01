#include "CollisionEngine.hpp"

glm::vec2 CollisionEngine::Reflect(glm::vec2 dir, glm::vec2 normal)
{
    return dir - (2.0f * (glm::dot(dir, normal))*dir);
}

CollisionEngine::CollisionEngine(std::shared_ptr<UI>& ui_ptr) : CollisionLoop{}
{
    m_ui_ptr = ui_ptr;
}

CollisionEngine::~CollisionEngine()
{
}

void CollisionEngine::RunCollisionLoop(bool check)
{
    if (!check)
    {
        return;
    }
    
    for (std::function<void()>& fun : CollisionLoop)
    {
        fun();
    }
}

void CollisionEngine::ResolvePlayerCircleToCicleCollision(std::shared_ptr<Boundary> &c1_ptr, std::shared_ptr<Boundary> &c2_ptr)
{
    if (!c1_ptr->CollideAgainstCircle(c2_ptr))
        return;

    m_ui_ptr->PlayBounce(c2_ptr->Origin());
    glm::vec2 delta_d{c2_ptr->Origin() - c1_ptr->Origin()};
    glm::vec2 n = glm::normalize(
        delta_d
    );
    float distance = glm::length(delta_d);

    glm::vec2 v1 = c1_ptr->Velocity();
    glm::vec2 v2 = c2_ptr->Velocity();

    float m1 = c1_ptr->Mass();
    float m2 = c2_ptr->Mass();
    float invMass1 = 1.0f / m1;
    float invMass2 = 1.0f / m2;

    float v_rel = glm::dot(v1 - v2, n);

    if (v_rel > 0.0f)
    {
        constexpr float restitution = 1.0f;

        float j =
            -(1.0f + restitution) * v_rel /
            (1.0f / m1 + 1.0f / m2);

        c1_ptr->SetVelocity(v1 + (j / m1) * n);
        c2_ptr->SetVelocity(v2 - (j / m2) * n);
    }
    else
    {
        c2_ptr->SetVelocity(
            glm::reflect(v2, n)
        );
    }

    auto circle_ptr1 = std::dynamic_pointer_cast<BoundaryCircle>(c1_ptr);
    if (!circle_ptr1)
    {
        std::cout << "CollisionEngine::ResolvePlayerCircleToCicleCollision: circle_ptr1!" << std::endl;
        return;
    }
    auto circle_ptr2 = std::dynamic_pointer_cast<BoundaryCircle>(c2_ptr);
    if (!circle_ptr2)
    {
        std::cout << "CollisionEngine::ResolvePlayerCircleToCicleCollision: circle_ptr2!" << std::endl;
        return;
    }
    float penetration = (circle_ptr1->Radius() + circle_ptr2->Radius()) - distance;
    if (penetration > 0.0f)
    {
        glm::vec2 correction = n * (penetration / (invMass1 + invMass2));
        c1_ptr->Tranaslate(-correction * invMass1);
        c2_ptr->Tranaslate(correction * invMass2);
        if (c2_ptr->TryTranaslate(correction * invMass2))
        {
            c1_ptr->Tranaslate(-correction * invMass1);
        }
        else
        {
            c1_ptr->Tranaslate(-n * penetration);
        }
    }
    SendBallEventsToServer();
}

void CollisionEngine::ResolveBoundaryLineToCicleCollision(std::shared_ptr<Boundary> &l1_ptr, std::shared_ptr<Boundary> &c2_ptr)
{
    if (!l1_ptr->CollideAgainstCircle(c2_ptr))
        return;

    m_ui_ptr->PlayBounce(c2_ptr->Origin());
    auto other_line = std::dynamic_pointer_cast<BoundaryLine>(l1_ptr);
    if (!other_line)
    {
        std::cout << "CollisionEngine::ResolveBoundaryLineToCicleCollision: other_line" << std::endl;
        return;
    }

    c2_ptr->Reflect(other_line->Normal());
}

void CollisionEngine::ResolvePlayerCicleXCollision(std::shared_ptr<Boundary> &c1_ptr, float max, float min)
{
    auto other_circle = std::dynamic_pointer_cast<BoundaryCircle>(c1_ptr);
    if (!other_circle)
    {
        std::cout << "BoundaryCircle::CollideAgainstCircle: other_circle" << std::endl;
        return;
    }

    if (other_circle->Origin().x + other_circle->Radius() > max)
    {
        other_circle->SetPosition(
            glm::vec2(
                max - other_circle->Radius(), other_circle->Origin().y
            )
        );
        other_circle->Reflect(glm::vec2(-1.0f, 0.0f));
    }

    if (other_circle->Origin().x - other_circle->Radius() < min)
    {
        other_circle->SetPosition(
            glm::vec2(
                min + other_circle->Radius(), other_circle->Origin().y
            )
        );
        other_circle->Reflect( glm::vec2(1.0f, 0.0f));
    }
}

void CollisionEngine::ResolvePlayerCicleZCollision(std::shared_ptr<Boundary> &c1_ptr, float max, float min)
{
    auto other_circle = std::dynamic_pointer_cast<BoundaryCircle>(c1_ptr);
    if (!other_circle)
    {
        std::cout << "BoundaryCircle::CollideAgainstCircle: other_circle" << std::endl;
        return;
    }

    if (other_circle->Origin().y + other_circle->Radius() > max)
    {
        other_circle->SetPosition(
            glm::vec2(
                other_circle->Origin().x,
                max - other_circle->Radius()
            )
        );
        other_circle->Reflect(glm::vec2(0.0f, -1.0f));
    }

    if (other_circle->Origin().y - other_circle->Radius() < min)
    {
        other_circle->SetPosition(
            glm::vec2(
                other_circle->Origin().x,
                min + other_circle->Radius()
            )
        );
        other_circle->Reflect(glm::vec2(0.0f, 1.0f));
    }
}

void CollisionEngine::ResolveGoalLineToCicleCollision(
        std::shared_ptr<Boundary>& l1_ptr,
        std::shared_ptr<Boundary>& c2_ptr,
        std::function<void()> on_goalscored
    )
{
    if (!l1_ptr->CollideAgainstCircle(c2_ptr))
        return;

    m_ui_ptr->PlayGoalSound();

    on_goalscored();
}
