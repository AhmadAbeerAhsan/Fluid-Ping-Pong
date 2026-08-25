#pragma once

#include <functional>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Boundary.hpp"
#include "../UI/UI.hpp"

class Controller
{
public:
    enum ControllerType
    {
        Keyboard_Player1,
        Keyboard_Player2,
        Mouse_Player,
        Bot,
        Online_Opponent
    };
private:
    std::shared_ptr<Boundary> m_boundary_ptr;

    int m_keyboard_controller_check{0};
public:
    Controller(ControllerType controller_type);
    ~Controller();

    std::function<void()> ListenInput;

    void AssignBoundary(std::shared_ptr<Boundary> boundary_ptr);

};
