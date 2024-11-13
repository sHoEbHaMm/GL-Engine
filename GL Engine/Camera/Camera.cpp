#include "Camera.h"

Camera::Camera() :
	cameraPos(0.0f, 0.0f, 6.0f), cameraUp(0.0f, 1.0f, 0.0f), cameraFront(0.0f, 0.0f, -1.0f),
    lastX(800/2.0f), lastY(600/2.0f), isFirstMouse(true), yaw(-90.0f), pitch(0.0f),
    MouseSensitivity(0.1f), Zoom(45.0f), MovementSpeed(2.5f)
{
	cameraView = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::processKeyboardInput(GLFWwindow* window, float deltaTime)
{
    const float cameraSpeed = MovementSpeed * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    cameraView = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::processMouseInput(GLFWwindow* window, double xpos, double ypos)
{
    if (isFirstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        isFirstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);

    cameraView = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::processZoom(GLFWwindow* window, double xoffset, double yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}
