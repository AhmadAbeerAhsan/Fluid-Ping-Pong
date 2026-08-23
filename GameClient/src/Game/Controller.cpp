#include "Controller.hpp"

Controller::Controller(ControllerType controller_type)
{
    switch (controller_type)
    {
    case ControllerType::Keyboard_Player1:
        ListenInput = std::function<void(GLFWwindow*)>{
            [this](GLFWwindow* window){
                glm::vec2 dir{0.0f, 0.0f};
                int keyboard_controller_check{0};
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                {
                    dir += glm::vec2(1.0f, 0.0f);
                    keyboard_controller_check += 1;
                }
                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                {
                    dir += glm::vec2(-1.0f, 0.0f);
                    keyboard_controller_check += 10;
                }
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                {
                    dir += glm::vec2(0.0f, -1.0f);
                    keyboard_controller_check += 100;
                }
                if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                {
                    dir += glm::vec2(0.0f, 1.0f);
                    keyboard_controller_check += 1000;
                }
                if (m_keyboard_controller_check != keyboard_controller_check)
                {
                    m_keyboard_controller_check = keyboard_controller_check;
                    m_boundary_ptr->SetDirection(glm::normalize(dir));
                }
            }
        };
        break;
    
    default:
        break;
    }
}

Controller::~Controller()
{
}

void Controller::AssignBoundary(std::shared_ptr<Boundary> boundary_ptr)
{
    m_boundary_ptr = boundary_ptr;
}
