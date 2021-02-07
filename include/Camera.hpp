#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace component::camera {
    struct data {
        float fov;
        float aspectRatio;
        float nearPlane;
        float farPlane;
    };

    entt::entity create(
        glm::vec3 position = {0,0,0}, 
        glm::quat rotation = {},
        float fov = 90.f,
        float aspectRatio = 4.0f / 3.0f,
        float nearPlane = 0.1f,
        float farPlane = 10000.f
    );

    glm::mat4 getProjection(entt::entity cam);
    glm::mat4 getView(entt::entity cam);
}
