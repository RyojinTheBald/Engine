#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Component
{
    struct PlayerControl
    {
        PlayerControl(glm::vec3 _direction = {0,0,0}, glm::quat _orientation = {0,0,0,0}): direction(_direction), orientation(_orientation) {}
        glm::vec3 direction;
        glm::quat orientation;
    };
}