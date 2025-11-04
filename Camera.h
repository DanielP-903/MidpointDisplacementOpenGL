#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include "shaderClass.h"
#include <imgui.h>
#include "HelperFunctions.h"

class Camera
{
public:
	// Camera position, forward and up vectors
	glm::vec3 Position;
	glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Combined camera matrix
	glm::mat4 camMatrix = glm::mat4(1.0f);

	// Camera pitch and yaw angles (in degrees)
	float pitch = -18.625f;
	float yaw = -133.875f;

	// Flag for ensuring the mouse doesn't snap upon first click
	bool firstClick = false;

	// Width and height of the camera's view frustum
	int width;
	int height;

	float fov = 45.0f;

	float near = 0.1f;
	float far = 100.0f;
	// wherever you are...

	// Speed of camera movement (WASD)
	float speed = 0.1f;

	// Sensitivity of the camera look (mouse)
	float sensitivity = 100.0f;

	// Construct a camera
	Camera(int cam_width, int cam_height, glm::vec3 cam_position);

	// Create our camera matrices that define how our camera looks and what shader it uses
	void UpdateMatrix(float FOVdeg, float nearPlane, float farPlane);
	void AssignMatrix(Shader& shader, const char* uniform);

	void SetCameraSize(int new_width, int new_height);

	// Process inputs across keyboard and mouse
	void Inputs(GLFWwindow* window);
	void ProcessKeyboardInput(GLFWwindow* window);
	void ProcessMouseInput(GLFWwindow* window);

};

#endif // !CAMERA_CLASS_H
