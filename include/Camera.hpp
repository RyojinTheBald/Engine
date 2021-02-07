#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera(
        glm::vec3 _position = {0, 0, 0},
        glm::vec3 _direction = {0, 0, 1},
        float fov = 90.0f,
        float aspectRatio = 4.0f / 3.0f,
        float nearPlane = 0.1f,
        float farPlane = 100.0f) : position(_position),
                                    direction(_direction)
    {
        updateView();
        updateProjection(fov, aspectRatio, nearPlane, farPlane);
    };

    void move(glm::vec3 direction)
    {
        setPosition(position + direction);
    }

    void rotate(float _pitch, float _yaw)
    {
        float pitch = asin(direction.y);
        float yaw = atan2(direction.x, direction.z);

        std::cout << "Old pitch/yaw: " << pitch << " " << yaw << std::endl;

        pitch += _pitch;
        yaw += _yaw;

        std::cout << "New pitch/yaw: " << pitch << " " << yaw << std::endl;

        glm::vec3 newDirection = {
            cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw)) * cos(glm::radians(pitch))
        };
        setDirection(glm::normalize(newDirection));
    }

    inline void setPosition(glm::vec3 _position)
    {
        // std::cout << "Old camera position: " << position.x << "," << position.y << "," << position.z << std::endl;
        // std::cout << "New camera position: " << _position.x << "," << _position.y << "," << _position.z << std::endl;
        position = _position;
        updateView();
    };

    inline void setDirection(glm::vec3 _direction)
    {
        direction = _direction;
        updateView();
    }

    inline void lookAt(glm::vec3 lookAt)
    {
        direction = glm::normalize(lookAt - position);
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

    [[nodiscard]] inline const glm::vec3 getPosition() { return position; };
    [[nodiscard]] inline const glm::vec3 getDirection() { return direction; };
    [[nodiscard]] inline const glm::mat4 getView() { return view; };
    [[nodiscard]] inline const glm::mat4 getProjection() { return projection; };

private:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;

    glm::mat4 view;
    glm::mat4 projection;
};