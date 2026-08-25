#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <memory>
#include <functional>

//#include "../Game/HomeScreen.hpp"
//#include "../Game/Match.hpp"

class UI
{
public:

    std::function<void(int, int)> Navigate_To_Match;
    std::function<void()> Navigate_To_HomeScreen;

private:
    ImGuiIO* m_io_ptr = nullptr;
public:
    UI(GLFWwindow* window);
    ~UI() = default;

    void SetupUI();
    void RenderUI();

    void CraftUI();
};
