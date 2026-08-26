add_library(
    UI
    UI.cpp
    AudioPlayer.cpp
    State.cpp
    UIWidgets.cpp
)

target_link_libraries(UI
    imgui
    miniaudio
    glm
)