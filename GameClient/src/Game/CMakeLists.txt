add_library(
    Game
    Boundary.cpp
    Controller.cpp
    CollisionEngine.cpp
    Match.cpp
)

target_link_libraries(Game
    glfw
    glm
    Renderer
)