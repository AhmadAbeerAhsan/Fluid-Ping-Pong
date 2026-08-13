add_library(
    Renderer
    Camera.cpp
    Shader.cpp
    Model.cpp
    Renderer.cpp
    Texture.cpp
)

target_link_libraries(Renderer
    glad
    glfw
    glm
)