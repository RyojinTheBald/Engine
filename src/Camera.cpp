#include "Camera.hpp"

#include "Registry.hpp"
namespace component::camera {
  entt::entity create(
      glm::vec3 position, 
      glm::quat rotation,
      float fov,
      float aspectRatio,
      float nearPlane,
      float farPlane
    )
  {
    const auto entity = registry.create();
    registry.assign<component::position>(entity, position);
    registry.assign<component::rotation>(entity, rotation);

    registry.assign<component::camera::data>(entity, fov, aspectRatio, nearPlane, farPlane);

    return entity;
  }


  glm::mat4 getProjection(entt::entity cam)
  {
    assert(registry.has<component::camera::data>(cam));

    auto camData = registry.get<component::camera::data>(cam);
    return glm::perspective(glm::radians(camData.fov), camData.aspectRatio, camData.nearPlane, camData.farPlane);
  }

  glm::mat4 getView(entt::entity cam)
  {
    auto rotation = registry.get<component::rotation>(cam);
    auto position = registry.get<component::position>(cam);
    return  glm::toMat4(rotation) * glm::translate(glm::mat4(1), position);
  }
}