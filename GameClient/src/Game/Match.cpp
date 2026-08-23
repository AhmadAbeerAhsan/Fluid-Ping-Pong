#include "Match.hpp"

Match::Match(
    Controller::ControllerType player1_type, Controller::ControllerType player2_type,
    std::shared_ptr<glm::ivec2> shared_resolution
) :
    m_player1(player1_type), m_player2(player2_type),
    m_shared_resolution(shared_resolution)
{
}

Match::~Match()
{
}

void Match::InitScene()
{
}
