#include "Camera.h"

// Construct a camera
Camera::Camera(int cam_width, int cam_height, glm::vec3 cam_position)
{
	width = cam_width;
	height = cam_height;
	Position = cam_position;
}

// Update our camera matrices that define how our camera looks
void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane) 
{
	// Define our model, view and projection matrices as identity matrices
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	fov = FOVdeg;
	near = nearPlane;
	far = farPlane;

	// Get our current view from our initial vectors
	view = glm::lookAt(Position, Position + Forward, Up);

	// Calculate the projection matrix based on the provided params
	projection = glm::perspective(glm::radians(fov), width / float(height), near, far);

	// Get resulting matrix
	camMatrix = projection * view;
}

void Camera::SetCameraSize(int new_width, int new_height)
{
	width = new_width;
	height = new_height;

	//UpdateMatrix(fov, near, far);
}


// Assign the camera matrix to the shader
void Camera::AssignMatrix(Shader& shader, const char* uniform)
{
	// Assign the camera matrix to the shader, ensuring it is placed in memory
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(camMatrix));
}

// Process inputs across keyboard and mouse
void Camera::Inputs(GLFWwindow* window)
{
	ProcessKeyboardInput(window);
	ProcessMouseInput(window);
}

void Camera::ProcessKeyboardInput(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)	// Forward
	{
		Position += speed * Forward;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)	// Left
	{
		Position += speed * -glm::normalize(glm::cross(Forward, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)	// Backward
	{
		Position += speed * -Forward;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)	// Right
	{
		Position += speed * glm::normalize(glm::cross(Forward, Up));
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)	// Up
	{
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)	// Down
	{
		Position += speed * -Up;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)	// Speed up
	{
		speed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)	// Return speed to normal
	{
		speed = 0.1f;
	}
}

void Camera::ProcessMouseInput(GLFWwindow* window)
{
	if (ImGui::GetIO().WantCaptureMouse)
	{
		// ImGUI has control over mouse
		return;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)	// Left click pressed
	{
		// Hide the cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// If it's the first click, auto set the cursor to the centre of the screen 
		// so we avoid any snapping that may occur if we don't
		if (firstClick)
		{
			firstClick = false;
			glfwSetCursorPos(window, width / 2.0f, height / 2.0f);
		}

		// Get the cursor position on screen
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		// Calculate mouse move detla x and y relative to the window
		float deltaX = (float)(x - (width / 2.0f)) / width;
		float deltaY = (float)(y - (height / 2.0f)) / height;

		// Update pitch and yaw of camera
		yaw += sensitivity * deltaX;
		pitch += sensitivity * -deltaY;

		// Clamp pitch
		pitch = glm::clamp(pitch, -89.0f, 89.0f);

		// Wrap yaw
		if (yaw > 180.0f) 
			yaw -= 360.0f;
		else if (yaw < -180.0f) 
			yaw += 360.0f;

		// Fixes gimbal lock
		Forward.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		Forward.y = sin(glm::radians(pitch));
		Forward.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		Forward = glm::normalize(Forward);

		// Reset the cursor to centre of the screen, 
		// ensuring it doesn't escape when we don't want it to
		glfwSetCursorPos(window, width / 2, height / 2);

	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)	// Released left click
	{
		// Show mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		// Reset first click flag (going to need to be clicked in again)
		firstClick = true;
	}
}
