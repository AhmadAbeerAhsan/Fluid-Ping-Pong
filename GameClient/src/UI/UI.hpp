#pragma once

#include "UIWidgets.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <functional>
#include <vector>
#include <string>

#include "AudioPlayer.hpp"
#include "../Connection/ClientServerContract.hpp"

class UI
{
public:
    std::function<void(int, int, int, GameSessionData)> Navigate_To_Match;
    std::function<void()> Navigate_To_HomeScreen;

    float DisplaySizeX(){ return m_io_ptr->DisplaySize.x; }
    float DisplaySizeY(){ return m_io_ptr->DisplaySize.y; }

private:
    ImGuiIO* m_io_ptr = nullptr;
    AudioPlayer m_audio_player;

public:
    UI(GLFWwindow* window);
    ~UI() = default;

    void SetupUI();
    void RenderUI();

    void PlayBgMusic(){ m_audio_player.playMusic(); }
    void StopBgMusic(){ m_audio_player.stopMusic(); }
    void PlayBounce(const glm::vec2& pos){ m_audio_player.playBounce(pos); }
    void PlayGoalSound(){ m_audio_player.playGoal(); }
    void PlayWinSound(){ m_audio_player.playWin(); }
    void PlayButtonSound(){ m_audio_player.playButton(); }
    float MusicVol(){ return m_audio_player.MusicVol(); };
    float SFXVol(){ return m_audio_player.SFXVol(); };
    float UIVol(){ return m_audio_player.UIVol(); };
    void SetMusicVol(float v){ return m_audio_player.setMusicVolume(v); };
    void SetSFXVol(float v){ return m_audio_player.setSfxVolume(v); };
    void SetUIVol(float v){ return m_audio_player.setUiVolume(v); };

    // Shared, consistently-styled widgets
    bool StyledButton(const char* label, ImVec2 size);
    bool CenteredButton(const char* label, float width, float height = 0.0f);
    void CenteredText(const char* text, float fontScale = 1.0f);
    void Label(const char* text, ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    // Match HUD -- left name, centered score, right name. No clock.
    bool m_home_requested{false};
    bool m_match_requested{false};
    void DrawScoreHUD(const std::string& leftName, int leftScore,
                       const std::string& rightName, int rightScore);

    bool m_show_audio_settings{true};
    bool m_show_settings{false};
    std::vector<std::string> Red_Controls_Button{"W", "S", "D", "A"};
    std::vector<std::string> Green_Controls_Button{"I", "L", "K", "J"};
    std::string CreateDirectionString(bool is_red);
    std::vector<std::string> Controls_Direction{"Forward", "Right", "Down", "Left"};
    
    std::string Username{""};
    void DrawGlobalSettings(std::function<void()> fun = {[](){}});
};
