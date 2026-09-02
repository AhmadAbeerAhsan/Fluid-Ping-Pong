#include "Controller.hpp"

void Controller::InitControllers()
{
    Keyboard_1_Listner  = std::function<void(const std::vector<glm::vec2>&)>{
        [this](const std::vector<glm::vec2>& c_dir){
            glm::vec2 dir{0.0f, 0.0f};
            int keyboard_controller_check{0};
            if (ImGui::IsKeyDown(ImGuiKey_W))
            {
                dir += c_dir[0];
                keyboard_controller_check += 1;
            }
            if (ImGui::IsKeyDown(ImGuiKey_D))
            {
                dir += c_dir[1];
                keyboard_controller_check += 10;
            }
            if (ImGui::IsKeyDown(ImGuiKey_S))
            {
                dir += c_dir[2];
                keyboard_controller_check += 100;
            }
            if (ImGui::IsKeyDown(ImGuiKey_A))
            {
                dir += c_dir[3];
                keyboard_controller_check += 1000;
            }
            if (glm::dot(dir, dir) > 0.0001)
                        m_boundary_ptr->SetUserVelocity(glm::normalize(dir));
                    else
                        m_boundary_ptr->SetUserVelocity(dir);
            if (m_keyboard_controller_check != keyboard_controller_check)
            {
                m_keyboard_controller_check = keyboard_controller_check;
                SendData(m_boundary_ptr->Origin(), m_boundary_ptr->Velocity(), m_player_type);
            }
        }
    };
    Keyboard_2_Listner  = std::function<void(const std::vector<glm::vec2>&)>{
        [this](const std::vector<glm::vec2>& c_dir){
            glm::vec2 dir{0.0f, 0.0f};
            int keyboard_controller_check{0};
            if (ImGui::IsKeyDown(ImGuiKey_I))
            {
                dir += c_dir[0];
                keyboard_controller_check += 1;
            }
            if (ImGui::IsKeyDown(ImGuiKey_L))
            {
                dir += c_dir[1];
                keyboard_controller_check += 10;
            }
            if (ImGui::IsKeyDown(ImGuiKey_K))
            {
                dir += c_dir[2];
                keyboard_controller_check += 100;
            }
            if (ImGui::IsKeyDown(ImGuiKey_J))
            {
                dir += c_dir[3];
                keyboard_controller_check += 1000;
            }
            if (glm::dot(dir, dir) > 0.0001)
                        m_boundary_ptr->SetUserVelocity(glm::normalize(dir));
                    else
                        m_boundary_ptr->SetUserVelocity(dir);
            if (m_keyboard_controller_check != keyboard_controller_check)
            {

                m_keyboard_controller_check = keyboard_controller_check;
                SendData(m_boundary_ptr->Origin(), m_boundary_ptr->Velocity(), m_player_type);
            }
        }
    };

    Mouse_Listner = std::function<void(const std::vector<glm::vec2>&)>{
        [this](const std::vector<glm::vec2>& pos){
            glm::vec2 dir = pos[0] - m_boundary_ptr->Origin();
            if (glm::dot(dir, dir) > 0.00001)
            {
                m_boundary_ptr->SetUserVelocity(glm::normalize(dir));
            }
            else
            {
                m_boundary_ptr->SetUserVelocity(glm::vec2(0.0f, 0.0f));
            }
        }
    };

    Online_Listner = std::function<void(const std::vector<glm::vec2>&)>{
        [this](const std::vector<glm::vec2>& pos){
            if(glm::dot(pos[0], pos[0]) < 0.01f)
                return;
            m_boundary_ptr->SetPosition(
                (
                    pos[0] +
                    ((float)(pos[2].x + pos[2].y) * pos[1])
                ),
                player_interpolation_duration_short_ms
            );
            m_boundary_ptr->SetVelocity(pos[1]);    
        }
    };

    SendData = std::function<void(const glm::vec2&, const glm::vec2&, GameEventData::ObjectType&)>{
        [](const glm::vec2& pos, const glm::vec2& vel, GameEventData::ObjectType& player_type){}
    };
}

Controller::Controller(GameEventData::ObjectType player_type, ControllerType controller_type):
    m_player_type(player_type),
    m_controller_type(controller_type)
{
    InitControllers();
    SetControllerType(controller_type);
}

Controller::~Controller()
{
}

void Controller::SetControllerType()
{
    switch (m_controller_type)
    {
    case ControllerType::Keyboard1:
        ListenInput = Keyboard_1_Listner;
        break;
    case ControllerType::Keyboard2:
        ListenInput = Keyboard_2_Listner;
        break;
    case ControllerType::Mouse:
        ListenInput = Mouse_Listner;
        break;
    case ControllerType::Online:
        ListenInput = Online_Listner;
        break;
    default:
        break;
    };
}

void Controller::AssignBoundary(std::shared_ptr<Boundary> boundary_ptr)
{
    m_boundary_ptr = boundary_ptr;
}
