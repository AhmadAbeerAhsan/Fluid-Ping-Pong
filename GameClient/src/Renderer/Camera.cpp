#include "Camera.hpp"

Camera::Camera(
        float height, float radius, float angle_d, glm::vec3 _target, glm::vec3 _up, glm::vec3 _right,
        std::shared_ptr<glm::ivec2> shared_resolution,
        float _fov, float _near_plane, float _far_plane
) :
    m_fov(_fov),
    m_near_z(_near_plane), m_far_z(_far_plane),
    m_shared_resolution(shared_resolution),
    m_height(height), m_radius(radius), m_angle_d(angle_d)
{
    proj = glm::perspective(glm::radians(m_fov), (float)m_shared_resolution->x / (float)m_shared_resolution->y, m_near_z, m_far_z);
    SetupPosition();
    target = _target;
    up = _up;
    right = _right;

    updateView();

    lastX = (float)m_shared_resolution->x/2;
    lastY = (float)m_shared_resolution->y/2;
}

void Camera::translate(glm::vec3 translate)
{
    position += translate;
}

void Camera::updatePositon(glm::vec3 newPosition)
{
    position = newPosition;
}

void Camera::changeDirection(glm::vec3 direction)
{
    target = direction;
}

void Camera::updateView()
{
    view = glm::lookAt(
        position, 
        glm::vec3(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    skyboxView = glm::mat4(glm::mat3(view));

    glm::vec2 n_xz_dir{glm::normalize(glm::vec2(-position.x, -position.z))}; 
    Controls_Vector = {
        n_xz_dir,
        glm::vec2( -n_xz_dir.y, n_xz_dir.x ),
        -n_xz_dir,
        glm::vec2( n_xz_dir.y, -n_xz_dir.x )
    };
}

void Camera::setupProjection(const float _fov, const float _aspect_ratio, const float _near, const float _far)
{
    m_fov = _fov;
    m_near_z = _near;
    m_far_z = _far;
    proj = glm::perspective(glm::radians(_fov), _aspect_ratio, _near, _far);
}

void Camera::Rotate(const float _yaw, const float _pitch, const float _roll)
{
}

void Camera::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed{1.0f};
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        translate(cameraSpeed * target);   
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        translate(-cameraSpeed * target);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        translate(-glm::normalize(glm::cross(target, up)) * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        translate(glm::normalize(glm::cross(target, up)) * cameraSpeed);

    updateView();
}

void Camera::muouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);    

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        if (!rotateCamera)
        {
            lastX = xpos;
            lastY = ypos;
            rotateCamera = true;
            return;
        }
    }
    else
    {
        rotateCamera = false;
        return;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    target = glm::normalize(front);
    right = glm::normalize(glm::cross(target, glm::vec3(0.0f, 1.0f, 0.0f)));
    up    = glm::normalize(glm::cross(right, target));
    updateView();
}

void Camera::updatePersprectiveProj()
{
    proj = glm::perspective(glm::radians(m_fov), (float)m_shared_resolution->x / (float)m_shared_resolution->y, m_near_z, m_far_z);
    rotateCamera = false;
}

void Camera::SetupPosition()
{
    position = glm::vec3(0.0f);
    position.y = m_height;
    position.x = m_radius * std::sin(glm::radians(m_angle_d));
    position.z = m_radius * std::cos(glm::radians(m_angle_d));
}
