#include "HomeScreen.hpp"

HomeScreen::HomeScreen(
    std::shared_ptr<glm::ivec2>& shared_resolution,
    std::shared_ptr<UI>& ui_ptr,
    std::string texture_path
) :
    GameScreen(shared_resolution, ui_ptr),
    m_texture(texture_path.c_str())
{
    std::cout << "Creating shader m_screen_texture_shader..." << std::endl;
    m_screen_texture_shader = Shader{"GameClient/src/Renderer/Shaders/screen_texture.vs.glsl", "GameClient/src/Renderer/Shaders/screen_texture.fs.glsl"};
    std::cout << "m_screen_texture_shader id: " << *m_screen_texture_shader.ID << std::endl;

    std::vector<glm::vec3> m_positions {};
    std::vector<glm::vec3> m_colors {};
    std::vector<glm::uvec3> m_indices {};
    std::vector<glm::vec2> m_tex_coords {};
    GenerateFullscreenQuad(m_positions, m_tex_coords, m_indices);
    fullscreen_quad.SetGeometry(m_positions, m_indices);
    fullscreen_quad.SetMaterial(m_texture, m_tex_coords);
    fullscreen_quad.SetShader(m_screen_texture_shader);
    fullscreen_quad.initializeForGL();
    fullscreen_quad.UpdateModelMatrix();
}

HomeScreen::~HomeScreen()
{
    std::cout << "HomeScreen::~HomeScreen()" << std::endl;
}

void HomeScreen::SetupUI()
{
    GameScreen::SetupUI();
}

void HomeScreen::DrawScene()
{
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_displayBuffer.Bind();

    m_screen_texture_shader.Activate();
    fullscreen_quad.DrawWithExternalShader(m_screen_texture_shader);

    m_displayBuffer.Unbind();
}

void HomeScreen::OnChangeResolution()
{
    GameScreen::OnChangeResolution();
}

void HomeScreen::OnMouseMoved(GLFWwindow *window_ptr, double xposIn, double yposIn)
{
}

void HomeScreen::OnKeyPressed(GLFWwindow *window_ptr)
{
    std::cout << "HomeScreen::OnKeyPressed(GLFWwindow *window_ptr)" << std::endl;
}

void HomeScreen::ListenKeysPressed()
{
}
