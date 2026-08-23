#include "GameScreen.hpp"

GameScreen::GameScreen(std::shared_ptr<glm::ivec2> shared_resolution) :
    m_shared_resolution(shared_resolution),
    m_displayBuffer(Framebuffer::FrameBufferType::Color_FloatAlpha, shared_resolution)
{
}

GameScreen::~GameScreen()
{
    std::cout << "GameScreen::~GameScreen()" << std::endl;
}

void GameScreen::UseScenceAsTexture()
{
    m_displayBuffer.BindTexture(GL_TEXTURE0);
}

void GameScreen::ClearScene()
{
    m_displayBuffer.ClearBufferForNextDraw();
}

void GameScreen::OnChangeResolution()
{
    m_displayBuffer.Resize();
}

void GameScreen::OnMouseMoved(GLFWwindow *window_ptr, double xposIn, double yposIn)
{
    std::cout << "GameScreen::OnMouseMoved" << std::endl;
}

void GameScreen::OnKeyPressed(GLFWwindow *window_ptr)
{
    std::cout << "GameScreen::OnKeyPressed" << std::endl;
}