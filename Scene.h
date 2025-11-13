#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#define PROJECT_DIR "G:\\Users\\Ping998\\Desktop\\Projects\\C++ Projects\\OpenGL\\MidpointDisplacementOpenGL"
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "Mesh.h"

class Scene
{
public:
	Scene(GLFWwindow* myWindow, int width, int height);

	virtual void PreInit();
	virtual void Init();
	virtual void PostInit();

	virtual void Input();
	virtual void Update(float deltaTime);
	virtual void Render();

	virtual void DebugRender();

	virtual void Shutdown();


private:
	int screenWidth;
	int screenHeight;

	static const int resolution = 6;
	float* heightMap = nullptr;

	float FPS = 0.0f;
	float scale = 1.0f;
	bool draw = true;
	bool wireframe = false;
	GLuint wireframeUni = 0;

	GLFWwindow* window;


	float colours[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float customLightPos[3] = { 0.5f, 0.5f, 0.5f };
	float directionalDir[3] = { 1.0f, 1.0f, 0.0f };

	// Define the light colour (RGBA)
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Define the light's position
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	float elapsedTime = 0.0f;
	float mag = 0.4f;
	float texScale = 1.0f;
	float frequency = 2.0f;
	float amplitude = 0.2f;
	float speed = 0.0f;
	float randomness = 1.0f;

	float attenuationQuadraticValue = 3.0f;
	float attenuationLinearValue = 0.7f;
	float attenuationConstantValue = 1.0f;

	Camera* mainCamera = nullptr;
	Shader* shaderProgram = nullptr;
	Shader* lightProgram = nullptr;
	Shader* normalsDisplayProgram = nullptr;
	Mesh* floor = nullptr;
	Mesh* light = nullptr;

	std::vector<Vertex> vertices_vec;
	std::vector<GLuint> indices_vec;
};

#endif