add_subdirectory(src/UI)
add_subdirectory(src/Renderer)
add_subdirectory(src/Game)

add_executable(
    GameClient
    src/main.cpp
)

target_include_directories(GameClient PRIVATE
    external/glad/include
)

target_link_libraries(GameClient
    glad
    glfw
    glm
    UI
    assimp
    ClientServerContract
    Renderer
    Game
)

# Linux extra OpenGL libraries
if(UNIX AND NOT APPLE)
    target_link_libraries(GameClient dl GL X11 pthread)
endif()

# Copy shaders to build output directory
add_custom_command(
    TARGET GameClient POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/GameClient/src/Renderer/Shaders
        $<TARGET_FILE_DIR:GameClient>/Shaders
)

# Copy assets to build output directory
add_custom_command(
    TARGET GameClient POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/GameClient/assets
        $<TARGET_FILE_DIR:GameClient>/assets
)