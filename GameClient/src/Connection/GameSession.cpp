#include "GameSession.hpp"

GameSession::GameSession() :
    m_data()
{
    m_session_type = SessionType::Offline;
    std::cout << "Game Session Started!" << std::endl;
}

GameSession::GameSession(const ConnectionBuf &buf, size_t buf_len) :
    m_data(buf, buf_len)
{
    m_session_type = SessionType::Online;
}

float GameSession::ElapsedSeconds() const
{
    return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_start_time).count();
}

std::string GameSession::WhoWon()
{
    if (m_player_red_score >= m_max_score)
    {
        return "Player Red Won";
    }
    if (m_player_green_score >= m_max_score)
    {
        return "Player Green Won";;
    }
    return "";
}

void GameSession::Reset()
{
    m_player_red_score = 0;
    m_player_green_score = 0;
}
