#define STB_IMAGE_IMPLEMENTATION

#include "Renderer/Renderer.hpp"
#include "Renderer/Model.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/PointLight.hpp"
#include "Renderer/Framebuffer.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/AppWindow.hpp"

#include "Game/Match.hpp"
#include "Game/HomeScreen.hpp"

#include "UI/UI.hpp"

#include <chrono>

int main()
{
    int screen_width {1920}, screen_height {1080};
    std::shared_ptr<glm::ivec2> shared_resolution{std::make_shared<glm::ivec2>(screen_width, screen_height)};

    AppWindow appWindow{shared_resolution};
    std::shared_ptr<UI> ui_ptr{std::make_shared<UI>(appWindow.GetWindowPtr())};

    std::shared_ptr<GameScreen> screen{std::make_shared<HomeScreen>(
        shared_resolution,
        ui_ptr,
        "GameClient/assets/textures/base.png"
    )};
        
    appWindow.UpdateResolutionSubscibedCallback = std::function<void()>{
        [&screen](){
            screen->OnChangeResolution();
        }
    };
    appWindow.MouseMovedCallback = std::function<void(GLFWwindow*, double, double)>{
        [&screen](GLFWwindow* window_ptr, double xposIn, double yposIn){
            screen->OnMouseMoved(window_ptr, xposIn, yposIn);
        }
    };
    appWindow.KeyPressedCallback = std::function<void(GLFWwindow*)>{
        [&screen](GLFWwindow* window_ptr){
            //screen->OnKeyPressed(window_ptr);
        }
    };

    screen.reset(new Match(
        Controller::Keyboard_Player1,
        Controller::Keyboard_Player2,
        shared_resolution,
        ui_ptr
    ));

    
    ui_ptr->Navigate_To_HomeScreen = std::function<void()>{
        [&screen, &shared_resolution, &ui_ptr](){
            screen.reset(new HomeScreen(
                shared_resolution,
                ui_ptr,
                "GameClient/assets/textures/base.png"
            ));
        }
    };

    ui_ptr->Navigate_To_Match = std::function<void(int, int)>{
        [&screen, &shared_resolution, &ui_ptr](
            int p1,
            int p2
        ){
            screen.reset(new Match(
                static_cast<Controller::ControllerType>(p1),
                static_cast<Controller::ControllerType>(p2),
                shared_resolution,
                ui_ptr
            ));
        }
    };

    std::cout << "Creating shader m_screen_texture_shader..." << std::endl;
    Shader m_screen_texture_shader = Shader{"GameClient/src/Renderer/Shaders/screen_texture.vs.glsl", "GameClient/src/Renderer/Shaders/screen_texture.fs.glsl"};
    std::cout << "m_screen_texture_shader id: " << *m_screen_texture_shader.ID << std::endl;

    std::vector<glm::vec3> m_positions {};
    std::vector<glm::vec3> m_colors {};
    std::vector<glm::uvec3> m_indices {};
    std::vector<glm::vec2> m_tex_coords {};
    GenerateFullscreenQuad(m_positions, m_tex_coords, m_indices);
    Model fullscreen_quad {};
    fullscreen_quad.SetGeometry(m_positions, m_indices);
    fullscreen_quad.SetMaterial(m_tex_coords);
    fullscreen_quad.SetShader(m_screen_texture_shader);
    fullscreen_quad.initializeForGL();
    fullscreen_quad.UpdateModelMatrix();

    try
    {
        while (!appWindow.shouldClose())
        {
            //models.back().m_time = std::chrono::duration<float, std::milli>(end - start).count()/10000.0f;

            //start = std::chrono::high_resolution_clock::now();
            appWindow.ProcessEvents();
            screen->ListenKeysPressed();

            //ui_ptr->SetupUI();
            //ui_ptr->CraftUI();

            screen->SetupUI();
            screen->DrawScene();
            
            m_screen_texture_shader.Activate();
            screen->UseScenceAsTexture();
            fullscreen_quad.DrawWithExternalShader(m_screen_texture_shader);
            screen->ClearScene();

            ui_ptr->RenderUI();
            appWindow.Display();
            appWindow.RecordEvents();

            screen->ProcessPendingNavigation();
        }
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    
    return 0;
}
