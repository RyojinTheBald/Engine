#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera(
        glm::vec3 _position = {0, 0, 0},
        glm::vec3 _direction = {0, 0, 1},
        float _fov = 90.0f,
        float _aspectRatio = 4.0f / 3.0f,
        float _nearPlane = 0.1f,
        float _farPlane = 100.0f) : position(_position),
                                    direction(_direction)
    {
        updateView();
        updateProjection(_fov, _aspectRatio, _nearPlane, _farPlane);
    };

    [[nodiscard]] inline const glm::vec3 getPosition() { return position; };
    [[nodiscard]] inline const glm::vec3 getDirection() { return direction; };

    inline void setPosition(glm::vec3 _position)
    {
        position = _position;
        updateView();
    };

    inline void setDirection(glm::vec3 _direction)
    {
        direction = _direction;
        updateView();
    }

    inline void updateProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
    {
        projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    };

    inline void updateView()
    {
        glm::vec3 right = glm::normalize(glm::cross({0.0f, 1.0f, 0.0f}, direction));
        up = glm::cross(direction, right);
        view = glm::lookAt(position, position + direction, up);
    };

    [[nodiscard]] inline const glm::mat4 getView() { return view; };
    [[nodiscard]] inline const glm::mat4 getProjection() { return projection; };

private:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;

    glm::mat4 view;
    glm::mat4 projection;
};