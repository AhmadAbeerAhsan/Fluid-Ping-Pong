#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <functional>

#include "AudioPlayer.hpp"
#include "State.hpp"

class UI
{
public:
    std::function<void(int, int)> Navigate_To_Match;
    std::function<void()> Navigate_To_HomeScreen;

    float DisplaySizeX(){ return m_io_ptr->DisplaySize.x; }
    float DisplaySizeY(){ return m_io_ptr->DisplaySize.y; }

private:
    ImGuiIO* m_io_ptr = nullptr;
    AudioPlayer m_audio_player;
    State m_app_state;
public:
    UI(GLFWwindow* window);
    ~UI() = default;

    void SetupUI();
    void RenderUI();

    // State Functions
    std::string Username(){ return m_app_state.Username(); }
    void SetUsername(std::string username){ m_app_state.SetUsername(username); }

    void PlayBgMusic(){ m_audio_player.playMusic(); }
    void StopBgMusic(){ m_audio_player.stopMusic(); }
    void PlayBounce(const glm::vec2& pos){ m_audio_player.playBounce(pos); }
    void PlayGoalSound(){ m_audio_player.playGoal(); }
    void PlayWinSound(){ m_audio_player.playWin(); }
    void PlayButtonSound(){ m_audio_player.playButton(); }

    // Shared overlay window (fullscreen, no background/decoration) --
    // used by any screen that draws centered content over the 3D scene.
    void BeginFullscreenOverlay(const char* name);
    void EndOverlay();

    // Shared, consistently-styled widgets
    bool StyledButton(const char* label, ImVec2 size);
    bool CenteredButton(const char* label, float width, float height = 0.0f);
    void CenteredText(const char* text, float fontScale = 1.0f);
    void Label(const char* text, ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    // Match HUD -- left name, centered score, right name. No clock.
    void DrawScoreHUD(const std::string& leftName, int leftScore,
                       const std::string& rightName, int rightScore);
};
