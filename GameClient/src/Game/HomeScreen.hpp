#pragma once

#include "GameScreen.hpp"
#include "Controller.hpp"
#include "Match.hpp"
#include "../Renderer/Texture.hpp"
#include "../Renderer/Model.hpp"
#include "../Renderer/Shader.hpp"
#include "../Utilities/LockFreeQueue.hpp"

using Task = std::function<void()>;

class HomeScreen : public GameScreen
{
private:
    Model EmptyModel{};
    
    char m_player_name[32] = "";
    char m_match_name[32] = "";
    bool m_show_matchmaking_menu{false};
    int c1, c2;
    Match::MatchType m_match_type;

    GameSessionData joining_session{};
    std::vector<GameSessionData> game_sessions{};
    void RefreshOnlineGameSessionList();
    void SendJoinReq(int match_id, int player_type);

    void InitScene();
    std::shared_ptr<Camera> m_camera_ptr;

public:
    HomeScreen(
        std::shared_ptr<glm::ivec2>& shared_resolution,
        std::shared_ptr<UI>& ui_ptr,
        std::shared_ptr<Connection>& con,
        std::shared_ptr<AssetLoader>& assets
    );
    ~HomeScreen() override;

    void SetupUI();
    void DrawMenu();
    void DrawOnlineMatchMakingMenu();
    void DrawScene() override;
    void OnChangeResolution() override;
    void OnMouseMoved(GLFWwindow* window_ptr, double xposIn, double yposIn) override;
    void OnKeyPressed(GLFWwindow* window_ptr) override;
    void ListenKeysPressed() override;
    void ProcessPendingNavigation() override;
    void Clean() override;
};
