#include "InputSystem.hpp"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../components/all.hpp"

void InputSystem::onKey(const Events::Key key) {
    //ignore repeat keypress events
    if (key.action == 2)
        return;

    std::cout << "Key: " << key.keyCode << " - " << key.action << std::endl;


    glm::vec3 direction (0,0,0);
    float roll = 0;

    //TODO: configurable keymappings
    switch(key.keyCode)
    {
        case GLFW_KEY_W:
            direction.z += 1 * (key.action? 1 : -1);
            break;
        case GLFW_KEY_S:
            direction.z -= 1 * (key.action? 1 : -1);
            break;
        case GLFW_KEY_A:
            direction.x += 1 * (key.action? 1 : -1);
            break;
        case GLFW_KEY_D:
            direction.x -= 1 * (key.action? 1 : -1);
            break;
        case GLFW_KEY_SPACE:
            direction.y -= 1 * (key.action? 1 : -1);
            break;
        case GLFW_KEY_LEFT_CONTROL:
            direction.y += 1 * (key.action? 1 : -1);
            break;
        case GLFW_KEY_E:
            roll += 1 * (key.action? 1 : -1);
            break;
        case GLFW_KEY_Q:
            roll -= 1 * (key.action? 1 : -1);
            break;
    }

    glm::quat orientation = glm::angleAxis((float)(roll * m_mouseSpeed), glm::vec3(0, 0, 1));

    // std::cout << "direction: " << direction.x << "," << direction.y << "," << direction.z << std::endl;
    
    

    //update player controlled entities with 
    for(auto &entity : m_registry->view<Component::PlayerControl>())
    {
        Component::PlayerControl oldControl = m_registry->get<Component::PlayerControl>(entity);
        // auto newDirection = glm::clamp<glm::vec3>(oldControl.direction + direction, glm::vec3(-1), glm::vec3(1));

        auto newDirection = glm::min(glm::max(oldControl.direction + direction, glm::vec3(-1)), glm::vec3(1));

        std::cout << "direction: " << newDirection.x << "," << newDirection.y << "," << newDirection.z << std::endl;
        m_registry->replace<Component::PlayerControl>(entity, newDirection, orientation * oldControl.orientation);
    }
}

void InputSystem::onMouseMove(const Events::MouseMove mouseMove) {

}

void InputSystem::onMouseButton(const Events::MouseButton mouseButton) {

}

void InputSystem::onMouseScroll(const Events::MouseScroll mouseScroll) {

}