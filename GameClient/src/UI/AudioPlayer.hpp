#pragma once

#include <iostream>
#include <vector>
#include <functional>

#include "miniaudio.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class AudioPlayer
{
private:
    ma_result m_result;
    ma_engine m_engine;
    ma_engine_config m_engine_config;
    ma_fence m_fence;

    ma_sound_group m_music_group;
    ma_sound_group m_sfx_group;
    ma_sound_group m_ui_group;

    ma_sound m_bg_sound;
    ma_sound m_bounce;
    ma_sound m_ball_boundary_collision;
    ma_sound m_button_sound;
    ma_sound m_goal;
    ma_sound m_win;

    bool m_initialized{false};

    void loadMusicResources();
    void loadSfxResources();
    void loadUiResources();

    void initEmptyFunctions();

    std::vector<std::function<void()>> SoundCleanUpCallBacks;
    std::vector<std::function<void()>> SoundGroupCleanUpCallBacks;
    std::vector<std::function<void()>> EngineCleanUpCallBacks;
public:
    AudioPlayer(/* args */);

    void init();
    void shutdown();

    std::function<void()> playMusic;
    std::function<void()> stopMusic;

    std::function<void()> playButton;
    std::function<void()> playGoal;
    std::function<void()> playWin;

    std::function<void(const glm::vec2&)> playBounce;

    std::function<void(float)> setMusicVolume;
    std::function<void(float)> setSfxVolume;
    std::function<void(float)> setUiVolume;
};
