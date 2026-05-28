#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#define PROJECT_DIR "G:\\Users\\Ping998\\Documents\\Projects\\C++ Projects\\OpenGL\\MidpointDisplacementOpenGL"
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "Event.h"
#include "Mesh.h"

using namespace glm;

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

	void TestFunc();

	virtual void DebugRender();

	void GenerateMidpointDisplacement();

	void RecalculatePositions();

	void RecalculateNormals();

	void MidpointDisplacement(int startValue = 0, bool useDefault = false);

	void DiamondStep(glm::ivec2 start, glm::ivec2 end);

	void SquareStep(glm::ivec2 start, glm::ivec2 end);

	float RandomAverage(float a, float b);

	float GenerateRandomHeight(float maxvalue);

	float GetHeight(glm::ivec2 position);

	float GetHeight(int x, int y);

	void SetHeight(glm::ivec2 position, float height);

	virtual void Shutdown();


private:
	int screenWidth;
	int screenHeight;

	static const int resolution = 9;
	float* heightMap = nullptr;

	// Corner values for midpoint displacement
	float cornerValues[4];
	float customCornerValues[4] = { 1, 1, -2, -2 };


	// Min and max range for heights (initial and changeable values)
	float SminRange = 0.0f;
	float SmaxRange = 10.0f;
	float minRange = SminRange;
	float maxRange = SmaxRange;

	float FPS = 0.0f;
	float objectScale = 1.0f;
	bool draw = true;
	bool wireframe = false;
	GLuint wireframeUni = 0;

	GLFWwindow* window;


	// Events
	Event<> testEvent;


	float colours[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float customLightPos[3] = { resolution / 2.0f, 5.0f, resolution / 2.0f };
	float directionalDir[3] = { 1.0f, 1.0f, 0.0f };

	// Define the light colour (RGBA)
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Define the light's position
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	float elapsedTime = 0.0f;
	float mag = 0.f;
	float texScale = 1.0f;
	float frequency = 2.0f;
	float amplitude = 0.2f;
	float speed = 0.0f;
	float randomness = 1.0f;

	float attenuationQuadraticValue = 0.01f;
	float attenuationLinearValue = 0.07f;
	float attenuationConstantValue = 0.01f;

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