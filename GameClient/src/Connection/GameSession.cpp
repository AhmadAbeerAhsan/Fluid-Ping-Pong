#include "GameSession.hpp"

GameSession::GameSession()
{
    m_start_time = std::chrono::steady_clock::now();
    std::cout << "Game Session Started!" << std::endl;
}

float GameSession::ElapsedSeconds() const
{
    return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_start_time).count();
}

std::string GameSession::WhoWon()
{
    if (m_player_red_score >= m_max_score)
    {
        return m_player_red_name;
    }
    if (m_player_green_score >= m_max_score)
    {
        return m_player_green_name;
    }
    return "";
}

void GameSession::Reset()
{
    m_player_red_score = 0;
    m_player_green_score = 0;
}
