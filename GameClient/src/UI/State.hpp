#pragma once

#include <string>

struct HomeState
{

};

struct MatchState
{

};

struct SettingsState
{
    bool show_audio_settings{true};
    bool show_red_controls_settings{true};
    bool show_green_controls_settings{true};
    bool show_camera_settings{true};
    bool show_game_settings{true};
    bool show_settings{false};
};

struct UserState
{

};

class State
{
private:
    std::string m_username{};
public:
    State();

    std::string Username(){ return m_username; };
    void SetUsername(std::string username){ m_username = username; };
};
