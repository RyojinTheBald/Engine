#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

extern entt::registry registry;

//simple common components live here
namespace component {
    typedef glm::vec3 position;
    typedef glm::quat rotation;
}