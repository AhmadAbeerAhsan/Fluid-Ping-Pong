#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <string>

#include "GameEvent.hpp"

class GameSession
{
public:
    enum SessionType{
        Offline,
        Online
    };

    GameSession();

    float ElapsedSeconds() const;
    void AddPointPlayerRed() { m_player_red_score++; };
    void AddPointPlayerGreen() { m_player_green_score++; };
    std::string WhoWon();

    std::string PlayerRedName() { return m_player_red_name; }
    std::string PlayerGreenName() { return m_player_green_name; }
    int PlayerRedScore() { return m_player_red_score; }
    int PlayerGreenScore() { return m_player_green_score; }
    int MaxScore() { return m_max_score; }

    void Reset();
private:
    std::string m_player_red_name{"Player Red"};
    std::string m_player_green_name{"Player Green"};
    int m_player_red_score{0};
    int m_player_green_score{0};
    int m_max_score{2};
    std::chrono::steady_clock::time_point m_start_time;

    std::vector<GameEvent> m_game_events {};
};