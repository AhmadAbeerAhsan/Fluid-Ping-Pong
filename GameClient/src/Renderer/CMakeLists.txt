add_library(
    Renderer
    Camera.cpp
    Shader.cpp
    Model.cpp
    Renderer.cpp
    Texture.cpp
    ObjectMaterial.cpp
    PointLight.cpp
    Framebuffer.cpp
    AppWindow.cpp
)

target_include_directories(Renderer PUBLIC
    ${CMAKE_SOURCE_DIR}/external/assimp/include   # the real headers (Importer.hpp, scene.h, ...)
    ${CMAKE_BINARY_DIR}/external/assimp/include    # config.h, generated at configure time
)

target_link_libraries(Renderer
    glad
    glfw
    glm
)