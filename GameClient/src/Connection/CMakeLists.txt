
# Boost installation
set(BOOST_ROOT "D:/boost_1_91_0")
set(Boost_NO_SYSTEM_PATHS ON)

add_library(connection
    Connection.cpp
    GameEvent.cpp
    GameEvent.hpp
    GameSession.cpp
    TcpClient.cpp
    UdpClient.cpp
    TcpConnection.cpp
)

target_include_directories(connection PUBLIC
    "D:/boost_1_91_0"
)

target_include_directories(connection PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Windows target version
if(WIN32)
    target_compile_definitions(
        connection
        PUBLIC
        _WIN32_WINNT=0x0601
    )
endif()