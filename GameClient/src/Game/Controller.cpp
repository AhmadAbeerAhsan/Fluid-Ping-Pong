#include "Controller.hpp"

Controller::Controller(ControllerType controller_type)
{
    switch (controller_type)
    {
    case ControllerType::Keyboard_Player1:
        ListenInput = std::function<void()>{
            [this](){
                glm::vec2 dir{0.0f, 0.0f};
                int keyboard_controller_check{0};
                if (ImGui::IsKeyDown(ImGuiKey_W))
                {
                    dir += glm::vec2(1.0f, 0.0f);
                    keyboard_controller_check += 1;
                }
                if (ImGui::IsKeyDown(ImGuiKey_S))
                {
                    dir += glm::vec2(-1.0f, 0.0f);
                    keyboard_controller_check += 10;
                }
                if (ImGui::IsKeyDown(ImGuiKey_A))
                {
                    dir += glm::vec2(0.0f, -1.0f);
                    keyboard_controller_check += 100;
                }
                if (ImGui::IsKeyDown(ImGuiKey_D))
                {
                    dir += glm::vec2(0.0f, 1.0f);
                    keyboard_controller_check += 1000;
                }
                if (glm::dot(dir, dir) > 0.0001)
                            m_boundary_ptr->SetUserVelocity(glm::normalize(dir));
                        else
                            m_boundary_ptr->SetUserVelocity(dir);
                if (m_keyboard_controller_check != keyboard_controller_check)
                {
                    //send data to server
                }
            }
        };
        break;
    case ControllerType::Keyboard_Player2:
        ListenInput = std::function<void()>{
            [this](){
                glm::vec2 dir{0.0f, 0.0f};
                int keyboard_controller_check{0};
                if (ImGui::IsKeyDown(ImGuiKey_I))
                {
                    dir += glm::vec2(1.0f, 0.0f);
                    keyboard_controller_check += 1;
                }
                if (ImGui::IsKeyDown(ImGuiKey_K))
                {
                    dir += glm::vec2(-1.0f, 0.0f);
                    keyboard_controller_check += 10;
                }
                if (ImGui::IsKeyDown(ImGuiKey_L))
                {
                    dir += glm::vec2(0.0f, 1.0f);
                    keyboard_controller_check += 100;
                }
                if (ImGui::IsKeyDown(ImGuiKey_J))
                {
                    dir += glm::vec2(0.0f, -1.0f);
                    keyboard_controller_check += 1000;
                }
                if (glm::dot(dir, dir) > 0.0001)
                            m_boundary_ptr->SetUserVelocity(glm::normalize(dir));
                        else
                            m_boundary_ptr->SetUserVelocity(dir);
                if (m_keyboard_controller_check != keyboard_controller_check)
                {
                    //send data to server
                }
            }
        };
        break;
    default:
        break;
    };
}

Controller::~Controller()
{
}

void Controller::AssignBoundary(std::shared_ptr<Boundary> boundary_ptr)
{
    m_boundary_ptr = boundary_ptr;
}
