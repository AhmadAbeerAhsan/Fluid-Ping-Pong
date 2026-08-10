#pragma once

#include <iostream>
#include <vector>

#include "GameEvent.hpp"

class GameSession
{
public:
    GameSession();
private:
    std::vector<GameEvent> m_game_events {};
};