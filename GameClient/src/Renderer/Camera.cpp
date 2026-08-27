#include "Camera.hpp"

Camera::Camera(
        float height, float radius, float angle_d,
        std::shared_ptr<glm::ivec2> shared_resolution,
        float _fov, float _near_plane, float _far_plane
) :
    m_fov(_fov),
    m_near_z(_near_plane), m_far_z(_far_plane),
    m_shared_resolution(shared_resolution),
    m_height(height), m_radius(radius), m_angle_d(angle_d)
{
    updatePersprectiveProj();
    SetupPosition();
    updateView();
}

void Camera::translate(glm::vec3 translate)
{
    position += translate;
}

void Camera::updatePositon(glm::vec3 newPosition)
{
    position = newPosition;
}

void Camera::updateView()
{
    view = glm::lookAt(
        position, 
        glm::vec3(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    skyboxView = glm::mat4(glm::mat3(view));
    inv_view = glm::inverse(view);

    glm::vec2 n_xz_dir{glm::normalize(glm::vec2(-position.x, -position.z))}; 
    Controls_Vector = {
        n_xz_dir,
        glm::vec2( -n_xz_dir.y, n_xz_dir.x ),
        -n_xz_dir,
        glm::vec2( n_xz_dir.y, -n_xz_dir.x )
    };
}

void Camera::updatePersprectiveProj()
{
    proj = glm::perspective(glm::radians(m_fov), (float)m_shared_resolution->x / (float)m_shared_resolution->y, m_near_z, m_far_z);
    rotateCamera = false;
    inv_proj = glm::inverse(proj);
}

void Camera::SetupPosition()
{
    position = glm::vec3(0.0f);
    position.y = m_height;
    position.x = m_radius * std::sin(glm::radians(m_angle_d));
    position.z = m_radius * std::cos(glm::radians(m_angle_d));
}
