#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <string>

#include "GameEvent.hpp"

class GameSession
{
public:
    GameSession();
    float ElapsedSeconds() const;
private:
    
    std::string m_player1_name{"Player 1"};
    std::string m_player2_name{"Player 2"};
    int m_player1_score{0};
    int m_player2_score{0};
    std::chrono::steady_clock::time_point m_start_time;

    std::vector<GameEvent> m_game_events {};
};