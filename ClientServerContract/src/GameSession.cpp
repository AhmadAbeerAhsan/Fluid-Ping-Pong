#include "GameSession.hpp"

GameSession::GameSession()
{
    std::cout << "Game Session Started!" << std::endl;
}

float GameSession::ElapsedSeconds() const
{
    return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_start_time).count();
}
