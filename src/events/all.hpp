#pragma once

namespace Events
{
    struct Key final
    {
        inline Key(int _keyCode, int _action, int _modifier) : keyCode(_keyCode), action(_action), modifier(_modifier)
        {
        }

        int keyCode = -1;
        int action = -1;
        int modifier = -1;
    };

    struct MouseMove final
    {
        inline MouseMove(int _x, int _y) : x(_x), y(_y) {}

        int x = -1, y = -1;
    };

    struct MouseButton final
    {
        inline MouseButton(int _button, int _action, int _mods) : 
            button(_button), action(_action), mods(_mods)
        {}

        int button = -1;
        int action = -1;
        int mods = -1;
    };

    struct MouseScroll final
    {
        inline MouseScroll(int _xOffset, int _yOffset) :
            xOffset(_xOffset), yOffset(_yOffset) 
        {}

        int xOffset = 0, yOffset = 0;
    };
} // namespace Events