add_library(
    UI
    UI.cpp
    AudioPlayer.cpp
    UIWidgets.cpp
)

target_link_libraries(UI
    imgui
    miniaudio
    glm
    connection
)