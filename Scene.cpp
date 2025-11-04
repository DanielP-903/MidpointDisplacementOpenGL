#include "Scene.h"
#include "HelperFunctions.h"
#include <random>
#include <imgui.h>

Scene::Scene(GLFWwindow* myWindow, int width, int height)
{
	window = myWindow;
	screenWidth = width;
	screenHeight = height;
}

void Scene::PreInit()
{
	std::cout << "Scene PreInit" << "\n";

	heightMap = new float[resolution * resolution];
}

void Scene::Init()
{
	std::cout << "Scene Init" << "\n";

	// Create our camera at the window's size
	mainCamera = new Camera(screenWidth, screenHeight, glm::vec3(12.31f, 5.58f, 12.89f));
	mainCamera->Forward = glm::vec3(-0.656790f, -0.319373f, -0.683102f);

	Texture textures[]
	{
		Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		//Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

	//BuildHeightMap();
	for (int j = 0; j < (resolution); j++)
	{
		for (int i = 0; i < (resolution); i++)
		{
			float value = ((float)(rand()) / ((float)RAND_MAX)) * (1.0f);
			heightMap[(j * resolution) + i] = ((float)rand() / (float)RAND_MAX) * 1.0f;
		}
	}
	
	float scale = 0.5f;

	constexpr size_t vertexCount = static_cast<size_t>(resolution) * resolution;
	constexpr size_t indexCount = static_cast<size_t>(((resolution - 1) * (resolution - 1))) * 6;

	// UV coords.
	int u = 0;
	int v = 0;
	int increment = 20.0f / resolution;

	int vertIndex = 0;

	std::unique_ptr<Vertex[]> vertices_gen(new Vertex[vertexCount]{});


	// Generate plane	
	for (int z = 0; z < resolution; z++)
	{
		for (int x = 0; x < resolution; x++)
		{
			float posX = (float)x;
			float posZ = (float)z;

			vertices_gen[vertIndex].position = glm::vec3(posX, heightMap[vertIndex], posZ);
			vertices_gen[vertIndex].UV = glm::vec2(u, v);

			// *

			u += increment;
			vertIndex++;
		}
		u = 0;
		v += increment;
	}

	std::unique_ptr<GLuint[]> indices_gen(new GLuint[indexCount]{});

	//Set up index list
	vertIndex = 0;
	for (int z = 0; z < resolution - 1; z++)
	{
		for (int x = 0; x < resolution - 1; x++)
		{
			//Build index array
			indices_gen[vertIndex] = (z * resolution) + x;
			indices_gen[vertIndex + 1] = ((z + 1) * resolution) + (x + 1);
			indices_gen[vertIndex + 2] = ((z + 1) * resolution) + x;

			// * * * * *
			// *     *
			// *   *
			// * *
			// *

			indices_gen[vertIndex + 3] = (z * resolution) + x;
			indices_gen[vertIndex + 4] = (z * resolution) + (x + 1);
			indices_gen[vertIndex + 5] = ((z + 1) * resolution) + (x + 1);

			//         *
			//       * *
			//     *   *
			//   *     *
			// * * * * *

			vertIndex += 6;
		}
	}


	// Create our shader program using our shader class and our defined default.vert and default.frag, vertex and fragment (pixel) shaders.
	shaderProgram = new Shader("default.vert", "default.frag");

	vertices_vec = std::vector<Vertex>(vertices_gen.get(), vertices_gen.get() + vertexCount);
	std::vector<GLuint> indices_vec(indices_gen.get(), indices_gen.get() + indexCount);
	std::vector<Texture> textures_vec(textures, textures + sizeof(textures) / sizeof(Texture));

	floor = new Mesh(vertices_vec, indices_vec, textures_vec);


	// Define the object's position
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 objectScale = glm::vec3(scale, 1.0f, scale);

	// Create an identity matrix for the object and translate it to the origin
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);
	objectModel = glm::scale(objectModel, objectScale);


	// Activate our base shader so we are using it
	shaderProgram->Activate();

	// Pass in our object's model and light colour and position to the shader
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	
	wireframeUni = glGetUniformLocation(shaderProgram->ID, "isWireframe");

}

void Scene::PostInit()
{
	std::cout << "Scene PostInit" << "\n";
}

void Scene::Input()
{	
	// Process camera input
	mainCamera->Inputs(window);
}

void Scene::Update(float deltaTime)
{
	//std::cout << "Scene Update" << "\n";

	FPS = 1.0f / deltaTime;

	elapsedTime += deltaTime;

	// Activate our base shader so we are using it
	shaderProgram->Activate();

	// Pass in our object's model and light colour and position to the shader
	glUniform1f(glGetUniformLocation(shaderProgram->ID, "time"), elapsedTime);
	glUniform1f(glGetUniformLocation(shaderProgram->ID, "freq"), frequency);
	glUniform1f(glGetUniformLocation(shaderProgram->ID, "amp"), amplitude);
	glUniform1f(glGetUniformLocation(shaderProgram->ID, "speed"), speed);


	// TODO: need a better way of doing this!
	// Ensure the camera is at the correct size
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	mainCamera->SetCameraSize(width, height);

	// Update camera matrices
	mainCamera->UpdateMatrix(45.0f, 0.1f, 100.0f);
}

void Scene::Render()
{
	if (!draw)
		return;	

	//std::cout << "Scene Render" << "\n";

	// Wireframe mode check
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
	glUniform1f(wireframeUni, wireframe);

	// Define the object's position
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 objectScale = glm::vec3(scale, 1.0f, scale);

	// Create an identity matrix for the object and translate it to the origin
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);
	objectModel = glm::scale(objectModel, objectScale);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));



	// --------
	// -- MESHES
	floor->Draw(*shaderProgram, *mainCamera);
}

void Scene::DebugRender()
{
	//std::cout << "Scene Debug Render" << "\n";

	ImGui::Text("%.1f FPS", FPS);

	ImGui::Checkbox("Draw?", &draw);

	// Wireframe
	ImGui::Checkbox("Wireframe Mode", &wireframe);

	ImGui::Text("-- Camera --");
	ImGui::Text("Camera Position: %f, %f, %f", mainCamera->Position.x, mainCamera->Position.y, mainCamera->Position.z);
	ImGui::Text("Camera Forward Vector: %f, %f, %f", mainCamera->Forward.x, mainCamera->Forward.y, mainCamera->Forward.z);
	ImGui::Text("Camera Pitch: %f", mainCamera->pitch);
	ImGui::Text("Camera Yaw: %f", mainCamera->yaw);
	ImGui::Text("-- Plane --");
	ImGui::DragFloat("Scale", &scale, 0.01f, 0.01f, 10.0f);
	ImGui::DragFloat("Frequency", &frequency, 0.01f, 0.01f, 10.0f);
	ImGui::DragFloat("Amplitude", &amplitude, 0.01f, 0.01f, 10.0f);
	ImGui::DragFloat("Speed", &speed, 0.01f, 0.01f, 10.0f);
	ImGui::DragFloat("Randomness", &randomness, 0.01f, 0.01f, 10.0f);
	if (ImGui::Button("Regenerate Height Map")) 
	{
		for (size_t i = 0; i < vertices_vec.size(); i++)
		{
			vertices_vec[i].position = glm::vec3(vertices_vec[i].position.x, ((float)(rand()) / ((float)RAND_MAX)) * (randomness), vertices_vec[i].position.z);
		}
		floor->Regenerate(vertices_vec);
	}
}

void Scene::Shutdown()
{
	std::cout << "Scene Shutdown" << "\n";

	// Delete all the objects we have created
	shaderProgram->Delete();

	delete shaderProgram;
	shaderProgram = nullptr;

	delete mainCamera;
	mainCamera = nullptr;

	delete floor;
	floor = nullptr;
}
