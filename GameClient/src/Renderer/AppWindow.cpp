#include "AppWindow.hpp"

AppWindow::AppWindow(std::shared_ptr<glm::ivec2> shared_resolution) :
m_shared_resolution(shared_resolution)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // get access to smaller subset of features without backward compatible features
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    //GLint stencilBits;
    //glGetIntegerv(GL_STENCIL_BITS, &stencilBits);
    //std::cout << "Stencil bits: " << stencilBits << std::endl;

    m_window_ptr = std::make_unique<GLFWwindow*>(glfwCreateWindow(m_shared_resolution->x, m_shared_resolution->y, "Learn OpenGl", nullptr, nullptr));
    if (*m_window_ptr == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(*m_window_ptr);
    glfwSetWindowUserPointer(*m_window_ptr, this);
    glfwSetFramebufferSizeCallback(*m_window_ptr, framebuffer_size_callback);
    glfwSetCursorPosCallback(*m_window_ptr, muouse_callback_static);

    //Hide Mouse Cursor: like fps
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);        
    
    // GLAD manages function pointers for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glViewport(0, 0, shared_resolution->x, shared_resolution->y); // tell OpenGl the size of the rendering window. First two parameters set the location of the lower left corner of the window
}

AppWindow::~AppWindow()
{
}

void AppWindow::RecordEvents()
{
    glfwPollEvents();
}

void AppWindow::ProcessEvents()
{
    KeyPressedCallback(*m_window_ptr);
}

void AppWindow::Display()
{
    glfwSwapBuffers(*m_window_ptr);
}

void AppWindow::UpdateWindowPreMature()
{
    int width, height;
    glfwGetWindowSize(*m_window_ptr, &width, &height);
    m_shared_resolution->x = width; m_shared_resolution->y = height;
    UpdateResolutionSubscibedCallback();
}

void AppWindow::framebuffer_size_callback(GLFWwindow *window, int _screen_width, int _screen_height)
{
    AppWindow* self = static_cast<AppWindow*>(glfwGetWindowUserPointer(window));
    if (self) {
        self->onFramebufferResize(_screen_width, _screen_height); // your actual instance logic
    }
}

void AppWindow::onFramebufferResize(int _screen_width, int _screen_height)
{
    m_shared_resolution->x = _screen_width;
    m_shared_resolution->y = _screen_height;
    glViewport(0, 0, m_shared_resolution->x, m_shared_resolution->y);

    UpdateResolutionSubscibedCallback();

}

void AppWindow::muouse_callback_static(GLFWwindow *window, double xposIn, double yposIn)
{
    AppWindow* self = static_cast<AppWindow*>(glfwGetWindowUserPointer(window));
    if (self) {
        self->muouse_callback(xposIn, yposIn); // your actual instance logic
    }
}

void AppWindow::muouse_callback(double xposIn, double yposIn)
{
    MouseMovedCallback(*m_window_ptr, xposIn, yposIn);
}