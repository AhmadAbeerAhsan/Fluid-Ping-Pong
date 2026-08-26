add_library(
    UI
    UI.cpp
    AudioPlayer.cpp
    State.cpp
)

target_link_libraries(UI
    imgui
    miniaudio
    glm
)