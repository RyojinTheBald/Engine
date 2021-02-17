#pragma once

#include <iostream>

#include "../core/Window.hpp"
#include "../events/all.hpp"

class InputSystem
{
    public:
        void onKey(const Events::Key key) {
            // std::cout << "Key: " << key.keyCode << std::endl;
        }

        void onMouseMove(const Events::MouseMove mouseMove) {

        }

        void onMouseButton(const Events::MouseButton mouseButton) {

        }

        void onMouseScroll(const Events::MouseScroll mouseScroll) {

        }
        
};