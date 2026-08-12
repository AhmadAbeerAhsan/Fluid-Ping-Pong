#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class Camera
{
public:
    glm::mat4 view;
    glm::mat4 proj;

    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    glm::vec3 right;
    
    Camera(
        glm::vec3 _position, glm::vec3 _target, glm::vec3 _up, glm::vec3 _right,
        float _screen_width, float _screen_height,
        float _fov, float _near_plane, float _far_plane
    );
    Camera() = default;

    // view
    void translate(glm::vec3 translate);
    void updatePositon(glm::vec3 newPosition);
    void changeDirection(glm::vec3 direction);
    void updateView();

    // proj
    void setupProjection(const float _fov, const float _aspect_ratio, const float _near = 0.1f, const float _far = 1000.0f);
    void Rotate(const float _yaw, const float _pitch, const float _roll);

    void processInput(GLFWwindow* &window, float &cameraSpeed);
    void muouse_callback(GLFWwindow *&window, double xposIn, double yposIn);

private:
    float yaw{}, pitch{}, roll{}, fov{}, aspect_ratio{}, near_z{}, far_z{};
    float lastX, lastY;

    bool rotateCamera{false};
    bool zoomedIn{false};
};
