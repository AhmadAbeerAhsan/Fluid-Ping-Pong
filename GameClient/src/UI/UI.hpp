#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <memory>
#include <functional>

class UI
{
private:
    ImGuiIO* m_io_ptr = nullptr;
public:
    UI(GLFWwindow* window);
    ~UI() = default;

    void SetupUI();
    void RenderUI();

    std::function<void()> CraftUI;
};
