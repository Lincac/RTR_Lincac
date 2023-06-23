#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

const float YAW = 90.0;
const float PITCH = 0.0;
const float ZOOM = 45.0;

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Yaw;
    float Pitch;
    float Zoom;
    float camera_length;
    float nearPlane;
    float farPlane;
    glm::vec3 center;

    Camera() {};

    Camera(glm::vec3 position = glm::vec3(), glm::vec3 front = glm::vec3(0, 0, -1), glm::vec3 up = glm::vec3(0, 1, 0), float yaw = YAW, float pitch = PITCH, float zoom = ZOOM)
        : WorldUp()
    {
        Position = position;
        Front = front;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        Zoom = zoom;
        camera_length = glm::length(position);
        nearPlane = 0.1f;
        farPlane = 100.0f;
        center = glm::vec3(0);

        updateCameraVectors();
    };

    glm::mat4 get_viewMatrix() { 
        return lookAt(Position, center, Up); 
    }

    void update(glm::vec3 newcenter) {
        Front = glm::vec3(0, 0, -1);
        WorldUp = glm::vec3(0, 1, 0);
        Yaw = YAW;
        Pitch = PITCH;
        Zoom = ZOOM;
        nearPlane = 0.1f;
        farPlane = 100.0f;
        center = newcenter;
        Position = newcenter + glm::vec3(0.0f, 0.0f, 4.0f);
        camera_length = glm::length(Position - newcenter);

        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset) {
        Zoom -= (float)yoffset; 
        Zoom = glm::clamp(1.0f, 89.0f, Zoom);
    }

    void ProcessMouseButton(float xoffset, float yoffset) {
        Yaw -= xoffset;
        Pitch -= yoffset;
        Pitch = glm::clamp(-88.0f, 89.0f, Pitch);

        Position.x = cos(glm::radians(Yaw)) * camera_length * cos(glm::radians(Pitch));
        Position.y = sin(glm::radians(Pitch)) * camera_length;
        Position.z = sin(glm::radians(Yaw)) * camera_length * cos(glm::radians(Pitch));
        Position += center;

        updateCameraVectors();
    }
protected:
    void updateCameraVectors() {
        Front = glm::normalize(center - Position);
        Right = glm::normalize(cross(Front, WorldUp));
        Up = glm::normalize(cross(Right, Front));
    }
};

#endif