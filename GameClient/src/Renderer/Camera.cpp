#include "Camera.hpp"

Camera::Camera(
        glm::vec3 _position, glm::vec3 _target, glm::vec3 _up, glm::vec3 _right,
        float _screen_width, float _screen_height,
        float _fov, float _near_plane, float _far_plane
) :
    fov(_fov), aspect_ratio(_screen_width/_screen_height), near_z(_near_plane), far_z(_far_plane)
{
    proj = glm::perspective(glm::radians(_fov), (float)_screen_width / (float)_screen_height, _near_plane, _far_plane);
    position = _position;
    target = _target;
    up = _up;
    right = _right;
    view = glm::lookAt(
        position, 
        position + target, 
  		up
    );

    lastX = _screen_width/2;
    lastY = _screen_height/2;
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
        position + target,
        up
    );
}

void Camera::setupProjection(const float _fov, const float _aspect_ratio, const float _near, const float _far)
{
    fov = _fov;
    aspect_ratio = _aspect_ratio;
    near_z = _near;
    far_z = _far;
    proj = glm::perspective(glm::radians(_fov), _aspect_ratio, _near, _far);
}

void Camera::Rotate(const float _yaw, const float _pitch, const float _roll)
{
}

void Camera::processInput(GLFWwindow *&window, float &cameraSpeed)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

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

void Camera::muouse_callback(GLFWwindow *&window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    /*s
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !zoomedIn)
    {
        std::cout << "zoomed in" << std::endl;
        setupProjection(fov/2, aspect_ratio, near_z, far_z);
        zoomedIn = true;
    }
    else if (zoomedIn)
    {
        std::cout << "zoomed out" << std::endl;
        setupProjection(fov * 2, aspect_ratio, near_z, far_z);
        zoomedIn = false;
    }
    */
    if (!zoomedIn)
    {
        std::cout << "zoomed in" << std::endl;
        setupProjection(fov/2, aspect_ratio, near_z, far_z);
        zoomedIn = true;
    }
    

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
