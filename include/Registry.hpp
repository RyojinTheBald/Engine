#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

extern entt::registry registry;

namespace component {
    typedef glm::vec3 position;
    typedef glm::quat rotation;
}