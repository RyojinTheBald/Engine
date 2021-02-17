#pragma once

#include <GLFW/glfw3.h>
#include <entt/entt.hpp>

#include "../core/Window.hpp"

class InputSystem
{
    public:
        void events(Core::Window window, entt::dispatcher dispatcher) {
            glfwPollEvents();
        }
};