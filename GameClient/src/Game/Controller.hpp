#pragma once

#include <functional>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Boundary.hpp"
#include "../UI/UI.hpp"
#include "ClientServerContract.hpp"

class Controller
{
public:
    enum ControllerType
    {
        Keyboard1,
        Keyboard2,
        Mouse,
        Bot,
        Online,
    };

private:
    std::shared_ptr<Boundary> m_boundary_ptr;

    int m_keyboard_controller_check{0};
    void InitControllers();

    std::function<void(const std::vector<glm::vec2>&)> Keyboard_1_Listner;
    std::function<void(const std::vector<glm::vec2>&)> Keyboard_2_Listner;
    std::function<void(const std::vector<glm::vec2>&)> Mouse_Listner;
    std::function<void(const std::vector<glm::vec2>&)> Online_Listner;

    GameEventData::ObjectType m_player_type;
    ControllerType m_controller_type;
public:
    Controller(GameEventData::ObjectType player_type, ControllerType controller_type);
    ~Controller();

    std::function<void(const std::vector<glm::vec2>&)> ListenInput;
    std::function<void(const glm::vec2&, const glm::vec2&, GameEventData::ObjectType&)> SendData;

    void SetControllerType();
    void AssignBoundary(std::shared_ptr<Boundary> boundary_ptr);

    void SetControllerType(ControllerType controller_type){ m_controller_type = controller_type; SetControllerType(); }
    ControllerType GetControllerType(){ return m_controller_type; }

};
