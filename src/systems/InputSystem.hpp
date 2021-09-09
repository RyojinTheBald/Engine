#pragma once

#include <iostream>
#include <entt/entt.hpp>

#include "../events/all.hpp"


class InputSystem
{
    public:
        InputSystem(entt::registry* registry) : m_registry(registry) {}

        void onKey(const Events::Key key);
        void onMouseMove(const Events::MouseMove mouseMove);
        void onMouseButton(const Events::MouseButton mouseButton);
        void onMouseScroll(const Events::MouseScroll mouseScroll);

        float m_mouseSpeed = 0.05f;

    private:
        entt::registry* m_registry;
        
};