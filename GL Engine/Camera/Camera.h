#pragma once
#include "config.h"

class Camera 
{

public:
	Camera();

public:
	void processKeyboardInput(GLFWwindow* window, float deltaTime);
	void processMouseInput(GLFWwindow* window, double xPos, double yPos);
	void processZoom(GLFWwindow* window, double xoffset, double yoffset);

private:
	glm::vec3 cameraPos, cameraFront, cameraUp;
	glm::mat4 cameraView;
	float MovementSpeed;

	/* Mouse */
	float lastX, lastY;
	bool isFirstMouse;
	float yaw;
	float pitch;
	float MouseSensitivity;
	float Zoom;

public:
	inline glm::mat4 GetCameraView() const { return cameraView; }
	inline float GetZoom() const { return Zoom;  }
	inline glm::vec3 GetCameraPos() const { return cameraPos; }
};
