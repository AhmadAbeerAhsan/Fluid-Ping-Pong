#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <memory>

#include "BoundaryLine.hpp"

class CollisionEngine
{
private:
    glm::vec2 Reflect(glm::vec2 dir, glm::vec2 normal);
    std::shared_ptr<UI> m_ui_ptr;
public:
    CollisionEngine(std::shared_ptr<UI>& ui_ptr);
    ~CollisionEngine();

    void RunCollisionLoop(bool check);
    std::vector<std::function<void()>> CollisionLoop;

    void ResolvePlayerCircleToCicleCollision(std::shared_ptr<Boundary>& c1_ptr, std::shared_ptr<Boundary>& c2_ptr);
    void ResolveBoundaryLineToCicleCollision(std::shared_ptr<Boundary>& l1_ptr, std::shared_ptr<Boundary>& c2_ptr);

    void ResolvePlayerCicleXCollision(std::shared_ptr<Boundary>& c1_ptr, float max, float min);
    void ResolvePlayerCicleZCollision(std::shared_ptr<Boundary>& c1_ptr, float max, float min);

    void ResolveGoalLineToCicleCollision(
        std::shared_ptr<Boundary>& l1_ptr,
        std::shared_ptr<Boundary>& c2_ptr,
        std::function<void()> on_goalscored
    );

    std::function<void()> SendBallEventsToServer;
};