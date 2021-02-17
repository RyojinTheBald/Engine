#pragma once

#include <glm/glm.hpp>

namespace Component {
    class Camera
    {
        public:
            Camera(
                float fov = 90.f,
                float aspectRatio = 4.0f / 3.0f,
                float nearPlane = 0.1f,
                float farPlane = 10000.f
            );

            glm::mat4 getProjection();

            float m_fov;
            float m_aspectRatio;
            float m_nearPlane;
            float m_farPlane;
    };
}