#include "Renderer.hpp"

Renderer::Renderer(int _screen_width, int _screen_height) :
    screen_width(_screen_width),
    screen_height(_screen_height)
{
    intializeGLFW(_screen_width, _screen_height);

    camera = Camera(
        glm::vec3(0.0f, 0.0f, -10.0f), 
  		glm::vec3(0.0f, 0.0f, 1.0f), 
  		glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        (float)_screen_width,
        (float)_screen_height,
        45.0f,
        0.01f, 1000.0f
    );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); //enable back face culling, by default only counter clock wise triangle are rendered
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    //enable wireframe
}

Renderer::~Renderer()
{
}

void Renderer::clear()
{
    timer.calculateDelta();
    processInput();
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::render()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

const glm::vec3 &Renderer::GetCameraPosition()
{
    return camera.position;
}

void Renderer::intializeGLFW(int width, int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // get access to smaller subset of features without backward compatible features
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    //GLint stencilBits;
    //glGetIntegerv(GL_STENCIL_BITS, &stencilBits);
    //std::cout << "Stencil bits: " << stencilBits << std::endl;

    window = glfwCreateWindow(width, height, "Learn OpenGl", nullptr, nullptr);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, Renderer::framebuffer_size_callback);
    glfwSetCursorPosCallback(window, muouse_callback_static);

    //Hide Mouse Cursor: like fps
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);        
    
    // GLAD manages function pointers for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glViewport(0, 0, width, height); // tell OpenGl the size of the rendering window. First two parameters set the location of the lower left corner of the window
}

void Renderer::processInput()
{
    float cameraSpeed = static_cast<float>(100 * timer.deltaTime);
    camera.processInput(window, cameraSpeed);
    
}

void Renderer::framebuffer_size_callback(GLFWwindow *window, int _screen_width, int _screen_height)
{
    Renderer* self = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    if (self) {
        self->onFramebufferResize(_screen_width, _screen_height); // your actual instance logic
    }
}

void Renderer::onFramebufferResize(int _screen_width, int _screen_height)
{
    screen_width = _screen_width;
    screen_height = _screen_height;
    glViewport(0, 0, screen_width, screen_height);
    // update projection matrix, aspect ratio, etc. — anything needing 'this'
}

void Renderer::muouse_callback_static(GLFWwindow *window, double xposIn, double yposIn)
{
    Renderer* self = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    if (self) {
        self->muouse_callback(xposIn, yposIn); // your actual instance logic
    }
}

void Renderer::muouse_callback(double xposIn, double yposIn)
{
    camera.muouse_callback(window, xposIn, yposIn);
}