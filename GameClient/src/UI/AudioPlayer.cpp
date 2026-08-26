#include "AudioPlayer.hpp"

void AudioPlayer::loadMusicResources()
{
    m_result = ma_sound_group_init(
        &m_engine, 0, NULL, &m_music_group
    );
    if (m_result != MA_SUCCESS) {
        std::cout << "AudioPlayer: Failed to initialize music sound group." << std::endl;
        return;
    }
    SoundGroupCleanUpCallBacks.emplace_back([this](){
        ma_sound_group_uninit(&m_music_group);
    });
    setMusicVolume = std::function<void(float)>{[this](float volume){
        ma_sound_group_set_volume(&m_music_group, volume);
    }};

    m_result = ma_sound_init_from_file(
        &m_engine,
        "GameClient/assets/sounds/bg.mp3",
        MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC,
        &m_music_group,
        &m_fence,
        &m_bg_sound
    );
    if (m_result != MA_SUCCESS) {
        std::cout << "AudioPlayer: Failed to initialize background music sound."
            << ma_result_description(m_result)
            << " (" << m_result << ")"
            << std::endl;
    }else {
        SoundCleanUpCallBacks.emplace_back([this](){
            ma_sound_uninit(&m_bg_sound);
        });
        ma_sound_set_looping(&m_bg_sound, MA_TRUE);

        playMusic = std::function<void()>{[this](){
            ma_sound_seek_to_pcm_frame(&m_bg_sound, 0);
            ma_sound_start(&m_bg_sound);
        }};
        stopMusic = std::function<void()>{[this](){
            ma_sound_stop(&m_bg_sound);
        }};
    }
}

void AudioPlayer::loadSfxResources()
{
    m_result = ma_sound_group_init(
        &m_engine, 0, NULL, &m_sfx_group
    );
    if (m_result != MA_SUCCESS) {
        std::cout << "AudioPlayer: Failed to initialize SFX sound group."
            << ma_result_description(m_result)
            << " (" << m_result << ")"
            << std::endl;
        return;
    }
    SoundGroupCleanUpCallBacks.emplace_back([this](){
        ma_sound_group_uninit(&m_sfx_group);
    });
    setSfxVolume = std::function<void(float)>{[this](float volume){
        ma_sound_group_set_volume(&m_sfx_group, volume);
    }};

    m_result = ma_sound_init_from_file(
        &m_engine,
        "GameClient/assets/sounds/bounce.mp3",
        MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC,
        &m_sfx_group,
        &m_fence,
        &m_bounce
    );
    if (m_result != MA_SUCCESS) {
        std::cout << "AudioPlayer: Failed to initialize player-ball collision sound."
            << ma_result_description(m_result)
            << " (" << m_result << ")"
            << std::endl;
    }else {
        SoundCleanUpCallBacks.emplace_back([this](){
            ma_sound_uninit(&m_bounce);
        });

        playBounce = std::function<void(const glm::vec2&)>{
            [this](const glm::vec2& position){
                ma_sound_stop(&m_bounce);

                ma_sound_seek_to_pcm_frame(
                    &m_bounce,
                    0
                );

                ma_sound_set_position(
                    &m_bounce,
                    position.x,
                    0.0f,
                    position.y
                );

                ma_sound_start(&m_bounce);
            }};
    
        ma_sound_set_pinned_listener_index(&m_bounce, 0);
        ma_sound_set_positioning(&m_bounce, ma_positioning_absolute);
        ma_sound_set_attenuation_model(&m_bounce, ma_attenuation_model_none);
        ma_sound_set_min_gain(&m_bounce, 1.0f);
        ma_sound_set_max_gain(&m_bounce, 1.0f);
    }
}

void AudioPlayer::loadUiResources()
{
    m_result = ma_sound_group_init(
        &m_engine, 0, NULL, &m_ui_group
    );
    if (m_result != MA_SUCCESS) {
        std::cout << "AudioPlayer: Failed to initialize UI sound group." << std::endl;
        return;
    }
    SoundGroupCleanUpCallBacks.emplace_back([this](){
        ma_sound_group_uninit(&m_ui_group);
    });
    setUiVolume = std::function<void(float)>{[this](float volume){
        ma_sound_group_set_volume(&m_ui_group, volume);
    }};

    m_result = ma_sound_init_from_file(
        &m_engine,
        "GameClient/assets/sounds/button.mp3",
        MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC,
        &m_ui_group,
        &m_fence,
        &m_button_sound
    );
    if (m_result != MA_SUCCESS) {
        std::cout << "AudioPlayer: Failed to initialize button sound." << std::endl;
    }else {
        SoundCleanUpCallBacks.emplace_back([this](){
            ma_sound_uninit(&m_button_sound);
        });

        playButton = std::function<void()>{[this](){
            ma_sound_seek_to_pcm_frame(&m_button_sound, 0);
            ma_sound_start(&m_button_sound);
        }};
    }

    m_result = ma_sound_init_from_file(
        &m_engine,
        "GameClient/assets/sounds/goal.wav",
        MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC,
        &m_ui_group,
        &m_fence,
        &m_goal
    );
    if (m_result != MA_SUCCESS) {
        std::cout << "AudioPlayer: Failed to initialize goal sound." << std::endl;
    }else {
        SoundCleanUpCallBacks.emplace_back([this](){
            ma_sound_uninit(&m_goal);
        });

        playGoal = std::function<void()>{[this](){
            ma_sound_seek_to_pcm_frame(&m_goal, 0);
            ma_sound_start(&m_goal);
        }};
    }

    m_result = ma_sound_init_from_file(
        &m_engine,
        "GameClient/assets/sounds/win.wav",
        MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC,
        &m_ui_group,
        &m_fence,
        &m_win
    );
    if (m_result != MA_SUCCESS) {
        std::cout << "AudioPlayer: Failed to initialize win sound." << std::endl;
    }else {
        SoundCleanUpCallBacks.emplace_back([this](){
            ma_sound_uninit(&m_win);
        });

        playWin = std::function<void()>{[this](){
            ma_sound_seek_to_pcm_frame(&m_win, 0);
            ma_sound_start(&m_win);
        }};
    }
}

void AudioPlayer::initEmptyFunctions()
{
    playMusic = std::function<void()>{[](){}};
    stopMusic = std::function<void()>{[](){}};

    playButton = std::function<void()>{[](){}};
    playGoal = std::function<void()>{[](){}};
    playWin = std::function<void()>{[](){}};

    playBounce = std::function<void(const glm::vec2&)>{[](const glm::vec2& pos){}};

    setMusicVolume = std::function<void(float)>{[](float v){}};
    setSfxVolume = std::function<void(float)>{[](float v){}};
    setUiVolume = std::function<void(float)>{[](float v){}};
}

AudioPlayer::AudioPlayer() : SoundCleanUpCallBacks{},
                             SoundGroupCleanUpCallBacks{},
                             EngineCleanUpCallBacks{}
{
    initEmptyFunctions();
}

void AudioPlayer::init()
{
    if (m_initialized)
    {
        return;
    }

    m_engine_config = ma_engine_config_init();

    m_result = ma_engine_init(&m_engine_config, &m_engine);
    if (m_result != MA_SUCCESS) {
        std::cout << "AudioPlayer::AudioPlayer: ailed to initialize the engine." << std::endl;
        return;
    }
    EngineCleanUpCallBacks.emplace_back([this](){
        ma_engine_uninit(&m_engine);
    });

    ma_engine_listener_set_position(&m_engine, 0, 0.0f, 0.0f, 0.0f);
    ma_engine_listener_set_direction(&m_engine, 0, 0.0f, 0.0f, 1.0f);
    ma_engine_listener_set_world_up(&m_engine, 0, 0.0f, 1.0f, 0.0f);

    m_result = ma_fence_init(&m_fence);
    if (m_result != MA_SUCCESS) {
        std::cout
            << "AudioPlayer: Failed to initialize audio fence. Error: "
            << m_result
            << std::endl;

        return;
    }

    loadMusicResources();
    loadSfxResources();
    loadUiResources();

    m_result = ma_fence_wait(&m_fence);
    if (m_result != MA_SUCCESS) {
        std::cout
            << "AudioPlayer: Failed while waiting for audio resources. Error: "
            << m_result
            << std::endl;
        ma_fence_uninit(&m_fence);
        return;
    }
    ma_fence_uninit(&m_fence);

    m_initialized = true;
}

void AudioPlayer::shutdown()
{
    m_initialized = false;
    for (auto it = SoundCleanUpCallBacks.rbegin();
         it != SoundCleanUpCallBacks.rend();
         ++it)
    {
        (*it)();
    }

    for (auto it = SoundGroupCleanUpCallBacks.rbegin();
         it != SoundGroupCleanUpCallBacks.rend();
         ++it)
    {
        (*it)();
    }

    for (auto it = EngineCleanUpCallBacks.rbegin();
         it != EngineCleanUpCallBacks.rend();
         ++it)
    {
        (*it)();
    }

    SoundCleanUpCallBacks.clear();
    SoundGroupCleanUpCallBacks.clear();
    EngineCleanUpCallBacks.clear();
}