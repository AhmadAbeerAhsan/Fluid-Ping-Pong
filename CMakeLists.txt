cmake_minimum_required(VERSION 3.20)
project(FluidPingPong CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(ClientServerContract)
add_subdirectory(GameServer)
add_subdirectory(GameClient)