#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <string>

#include "GameEvent.hpp"
#include "ClientServerContract.hpp"

class GameSession
{
public:
    enum SessionType{
        Offline,
        Online
    };

    GameSession();
    GameSession(const ConnectionBuf &buf, size_t buf_len);

    GameSessionData m_data;

    float ElapsedSeconds() const;
    void AddPointPlayerRed() { m_player_red_score++; };
    void AddPointPlayerGreen() { m_player_green_score++; };
    std::string WhoWon();

    int PlayerRedScore() { return m_player_red_score; }
    int PlayerGreenScore() { return m_player_green_score; }
    int MaxScore() { return m_max_score; }

    void Reset();
private:
    int m_player_red_score{0};
    int m_player_green_score{0};
    int m_max_score{2};
    SessionType m_session_type;
    std::chrono::steady_clock::time_point m_start_time;
};