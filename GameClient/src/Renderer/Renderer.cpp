#include "Renderer.hpp"

Renderer::Renderer(std::shared_ptr<glm::ivec2> shared_resolution, std::shared_ptr<Camera> camera) :
    m_shared_resolution(shared_resolution),
    m_camera(camera)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); //enable back face culling, by default only counter clock wise triangle are rendered
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    //enable wireframe
}

Renderer::~Renderer()
{
}

void Renderer::clear()
{
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::render()
{
}