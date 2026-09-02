#pragma once

#include "Controller.hpp"
#include "GameScreen.hpp"
#include "BoundaryCircle.hpp"
#include "BoundaryLine.hpp"
#include "CollisionEngine.hpp"
#include "../Renderer/Renderer.hpp"
#include "../Renderer/PointLight.hpp"
#include "LocalPlayerGameEventWindow.hpp"
#include <random>
#include <chrono>

class Match : public GameScreen
{
public:
    enum MatchType
    {
        Offline,
        Online
    };

private:
    Controller m_player_red, m_player_green;
    Controller::ControllerType old_red_keyboard, old_green_keyboard;
    std::shared_ptr<Boundary> 
        m_boundary_red_player_ptr, m_boundary_green_player_ptr, m_boundary_online_player_ptr,
        m_boundary_ball_ptr,
        m_boundary_left_ptr, m_boundary_right_ptr,
        m_boundary_red_goal_ptr, m_boundary_green_goal_ptr,
        m_boundary_bottomleft_ptr, m_boundary_bottomright_ptr,
        m_boundary_topleft_ptr, m_boundary_topright_ptr;

    //Game Objects
    std::shared_ptr<Camera> m_camera_ptr;

    Framebuffer m_snapshotBuffer;

    PointLight m_pointLight;

    Texture m_cube_map_texture;

    Shader m_blinn_phong_shdader;
    Shader m_shadow_map_shdader;
    Shader m_screen_texture_shader;
    Shader m_texture_cubemap_shdader;

    std::vector<std::shared_ptr<Model>> m_models{};
    Model m_cube_skybox{};
    Model m_floor{};

    float m_ball_radius{2.0f};
    float m_handle_radius{};

    CollisionEngine m_collision_engine;
    GameSessionData m_game_session_data;
    MatchType m_match_type;
    GameEventData::ObjectType m_local_player_type, m_online_player_type;
    GameEventData m_last_opponent_game_event{};
    GameEventData m_last_recieved_ball_game_event{};
    std::vector<glm::vec2> online_inputs{3, glm::vec2(0.0f, 0.0f)};
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point now;
    LocalPlayerGameEventWindow m_local_game_event_window{};
    void ReadOnlineEvents();
    void ProcessLocalPlayerEvents(GameEventData& e);
    void ProcessOnlinePlayerEvents(GameEventData& e);
    void ProcessRecievedBallEvents(GameEventData& e);

    //UI
    bool m_show_red_controls_settings{true};
    bool m_show_green_controls_settings{true};
    bool m_show_camera_settings{true};
    bool m_show_game_settings{true};
    bool m_settings_requested{false};
    bool m_match_running{false};
    std::string m_winner_name{""};

    void InitScene();
    void SetUpCollisionEngine();

public:
    Match(
        Controller::ControllerType player1_controller, Controller::ControllerType player2_controller,
        std::shared_ptr<glm::ivec2>& shared_resolution,
        std::shared_ptr<UI>& ui_ptr,
        std::shared_ptr<Connection>& con,
        MatchType match_type,
        GameSessionData game_session_data
    );
    ~Match() override;

    
    void DrawScene() override;
    void OnChangeResolution() override;
    void OnMouseMoved(GLFWwindow* window_ptr, double xposIn, double yposIn) override;
    void OnKeyPressed(GLFWwindow* window_ptr) override;
    void ListenKeysPressed() override;
    void ProcessPendingNavigation() override;

    void SetupUI() override;
    void SetupScoreBar();
    void SetupBottomMenu();

private:
    float lenght{100.0f};
    float width{50.0f};
    float goal_lenght{20.0f};
    float side_border_lenght{75.0f};
    float min_size{3.0f};

    std::vector<glm::vec2> m_red_spawn_points;
    std::vector<glm::vec2> m_green_spawn_points;
    std::mt19937 m_generator;

    void SpawnRedWithServe();
    void SpawnGreenWithServe();

    void SpawnRedWithoutServe(int inverse_r);
    void SpawnGreenWithoutServe(int inverse_r);

    void RedScored();
    void GreenScored();
    void DetermineWinner();

    void InitMatch();

    int RandomInt(int min, int max);

    std::function<const std::vector<glm::vec2>()> PassMouseXZPos;
    std::function<const std::vector<glm::vec2>()> PassCameraControls;
    std::function<const std::vector<glm::vec2>()> PassOnlineControls;
    void GiveMouseControls(GameEventData::ObjectType player_type);
    std::function<const std::vector<glm::vec2>()> PassRedInputs;
    std::function<const std::vector<glm::vec2>()> PassGreenInputs;
    std::function<void(const glm::vec2&, const glm::vec2&, GameEventData::ObjectType&)> SendLocalPlayerDataToServer;
    std::function<void()> SendBallEventsToServer;
    std::function<bool(float)> IsBallInOnlineSide;
    void InitializePassInputs();
    void DeterminePassInputs();
};
