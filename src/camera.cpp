#include "camera.h"
#include <iostream>
using namespace std;

// Constructor
Camera::Camera()
{
	position = glm::vec3(0.0f, 0.0f, 5.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = -90.0f;
	pitch = 0.0f;
	movementSpeed = 0.1f;
	mouseSensitivity = 0.1f;
	zoom = 45.0f;
	updateCameraVectors();
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

// Processes input received from any keyboard-like input system
void Camera::ProcessKeyboard(Camera_Movement direction)
{
	if (direction == FORWARD)
	{
		position += front * movementSpeed;
	}
	if (direction == BACKWARD)
	{
		position -= front * movementSpeed;
	}
	if (direction == LEFT)
	{
		position -= right * movementSpeed;
	}
	if (direction == RIGHT)
	{
		position += right * movementSpeed;
	}
}

// Processes input received from a mouse input system
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event
void Camera::ProcessMouseScroll(float yoffset)
{
	if (zoom >= 1.0f && zoom <= 45.0f)
		zoom -= yoffset;
	if (zoom <= 1.0f)
		zoom = 1.0f;
	if (zoom >= 45.0f)
		zoom = 45.0f;
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
	glm::vec3 f;
	f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	f.y = sin(glm::radians(pitch));
	f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(f);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}