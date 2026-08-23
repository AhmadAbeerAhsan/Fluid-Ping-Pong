#pragma once

#include "Controller.hpp"

class Match
{
private:
    Controller m_player1, m_player2;
    std::shared_ptr<glm::ivec2> m_shared_resolution;
public:
    Match(
        Controller::ControllerType player1_type, Controller::ControllerType player2_type,
        std::shared_ptr<glm::ivec2> shared_resolution
    );
    ~Match();

    void InitScene();
    void DrawScene();
    void ListenEvents();
};
