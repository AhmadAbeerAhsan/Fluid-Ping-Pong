cmake_minimum_required(VERSION 3.20)
project(FluidPingPong CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add GLFW
add_subdirectory(external/glfw)

# GLAD library
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# Add GLM (header-only, so INTERFACE — no compiled code)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE external/glm)

# Add imgui
add_subdirectory(external/imgui)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/assimp)

add_subdirectory(ClientServerContract)
add_subdirectory(GameServer)
add_subdirectory(GameClient)