add_library(ClientServerContract STATIC
    src/GameEvent.cpp
    src/GameEvent.hpp
    src/GameSession.cpp
    src/GameSession.hpp
)

target_include_directories(ClientServerContract PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
