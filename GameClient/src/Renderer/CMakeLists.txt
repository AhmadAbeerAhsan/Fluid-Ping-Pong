add_library(
    Renderer
    Camera.cpp
    Shader.cpp
    Model.cpp
    Renderer.cpp
)

target_link_libraries(Renderer
    glad
    glfw
    glm
)