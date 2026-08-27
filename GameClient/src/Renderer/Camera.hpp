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

    glm::mat4 inv_view;
    glm::mat4 inv_proj;

    glm::vec3 position;   
    Camera(
        float height, float radius, float angle_d,
        std::shared_ptr<glm::ivec2> shared_resolution,
        float _fov, float _near_plane, float _far_plane
    );
    Camera() = default;

    // view
    void translate(glm::vec3 translate);
    void updatePositon(glm::vec3 newPosition);
    void updateView();

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
    float m_fov{}, m_near_z{}, m_far_z{};

    std::shared_ptr<glm::ivec2> m_shared_resolution;

    bool rotateCamera{false};
};
