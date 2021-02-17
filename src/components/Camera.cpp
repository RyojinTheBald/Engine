#include "Camera.hpp"

#include <glm/gtx/transform.hpp>


namespace Component
{
    Camera::Camera(
        float fov,
        float aspectRatio,
        float nearPlane,
        float farPlane
    ) :
        m_fov {fov}, m_aspectRatio {aspectRatio}, m_nearPlane{nearPlane}, m_farPlane{farPlane}
    {
    }

    glm::mat4 Camera::getProjection()
    {
        return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
    }
}