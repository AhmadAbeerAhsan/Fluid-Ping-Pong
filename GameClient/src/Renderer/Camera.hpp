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
    glm::mat4 skyboxView;

    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    glm::vec3 right;
    
    Camera(
        float height, float radius, float angle_d, glm::vec3 _target, glm::vec3 _up, glm::vec3 _right,
        std::shared_ptr<glm::ivec2> shared_resolution,
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

    void processInput(GLFWwindow* window);
    void muouse_callback(GLFWwindow*, double xposIn, double yposIn);

    void updatePersprectiveProj();
    std::vector<glm::vec2> Controls_Vector{
        glm::vec2(-1.0f, 0.0f),
        glm::vec2( 0.0f,-1.0f),
        glm::vec2( 1.0f, 0.0f),
        glm::vec2( 0.0f, 1.0f)
    };

    float m_height, m_radius, m_angle_d;
    float GHeight(){ return m_height; }
    float GRadius(){ return m_radius; }
    float GAngle(){ return m_angle_d;}
    void SetupPosition();
    void SetGHeight(float h){ m_height = h; SetupPosition(); updateView(); }
    void SetGRadius(float r){ m_radius = r; SetupPosition(); updateView(); }
    void SetGAngle(float delta){ m_angle_d = delta; SetupPosition(); updateView(); }

private:
    float yaw{}, pitch{}, roll{}, m_fov{}, m_near_z{}, m_far_z{};
    float lastX, lastY;

    std::shared_ptr<glm::ivec2> m_shared_resolution;

    bool rotateCamera{false};
};
