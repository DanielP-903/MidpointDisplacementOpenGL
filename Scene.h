#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

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

	static const int resolution = 128;
	float* heightMap = nullptr;

	float FPS = 0.0f;
	float scale = 0.5f;
	bool draw = true;
	bool wireframe = false;
	GLuint wireframeUni = 0;

	GLFWwindow* window;

	float elapsedTime = 0.0f;
	float texScale = 1.0f;
	float frequency = 2.0f;
	float amplitude = 0.2f;
	float speed = 1.0f;
	float randomness = 1.0f;

	Camera* mainCamera = nullptr;
	Shader* shaderProgram = nullptr;
	Mesh* floor = nullptr;

	std::vector<Vertex> vertices_vec;
};

#endif