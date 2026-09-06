#include "GameScreen.hpp"

GameScreen::GameScreen(
        std::shared_ptr<glm::ivec2>& shared_resolution,
        std::shared_ptr<UI>& ui_ptr,
        std::shared_ptr<Connection>& con,
        int samples
    ) :
    m_displayBuffer(Framebuffer::FrameBufferType::Color_FloatAlpha, shared_resolution, samples),
    m_resolveBuffer(Framebuffer::FrameBufferType::Color_FloatAlpha, shared_resolution, 0)
{
    m_shared_resolution = shared_resolution;
    m_ui = ui_ptr;
    m_con = con;
}

GameScreen::~GameScreen()
{
    std::cout << "GameScreen::~GameScreen()" << std::endl;
}

void GameScreen::SetupUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GameScreen::UseScenceAsTexture()
{
    if (m_displayBuffer.IsMultisampled())
    {
        m_resolveBuffer.CopyFrom(m_displayBuffer);
        m_resolveBuffer.BindTexture(GL_TEXTURE0);
    }
    else
    {
        m_displayBuffer.BindTexture(GL_TEXTURE0);
    }
}

void GameScreen::ClearScene()
{
    m_displayBuffer.ClearBufferForNextDraw();
}

void GameScreen::OnChangeResolution()
{
    m_displayBuffer.Resize();
    m_resolveBuffer.Resize();
}