
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <random>
#include "Mesh.h"


int WINDOW_SIZE_X = 800;
int WINDOW_SIZE_Y = 800;


// REFERENCE: https://graphicscompendium.com/opengl/22-interpolation
static float lerp(float x, float y, float t)
{
	return x * (1.f - t) + y * t;
}
static glm::vec3 lerp(glm::vec3 x, glm::vec3 y, float t)
{
	return x * (1.f - t) + y * t;
}

// Helper to get a position on a circle given a centre point, radius and angle
static glm::vec2 positionOnCircle(glm::vec2 centre, float radius = 5.0f, float angle = 0.0f)
{
	float coords[] = {0.0f, 0.0f};
	coords[0] = radius * glm::cos(glm::radians(angle)) + centre.x;
	coords[1] = radius * glm::sin(glm::radians(angle)) + centre.y;

	return glm::make_vec2(coords);
}

void Callback_WindowSize(GLFWwindow* window, int width, int height)
{
	WINDOW_SIZE_X = width;
	WINDOW_SIZE_Y = height;
	glViewport(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);
}

static const int resolution = 16;
static float* heightMap = new float[resolution * resolution];

void BuildHeightMap()
{
	for (int j = 0; j < (resolution); j++) 
	{
		for (int i = 0; i < (resolution); i++) 
		{
			float value = ((float)(rand()) / ((float)RAND_MAX)) * (1.0f);
			heightMap[(j * resolution) + i] = ((float)rand() / (float)RAND_MAX) * 1.0f; // glm::sin(((j * resolution) + i) / (resolution * resolution));
		}
	}
}


int main()
{
	// Initialise GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using (currently we are using 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Setup GLFW to use the CORE profile - modern functions only
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// MSAA
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFWwindow object for displaying the application window at a predefined size
	GLFWwindow* window = 
		glfwCreateWindow(
			WINDOW_SIZE_X, // Size X
			WINDOW_SIZE_Y, // Size Y
			"My OpenGL Project",	// Title
			NULL, // Is Fullscreen
			NULL
	);

	// Failsafe check to ensure the window created successfully
	if (window == NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set the current context (i.e. tell GLFW to get a move on and use the window we made)
	glfwMakeContextCurrent(window);

	// Use glad to load and configure OpenGL
	gladLoadGL();

	// Create the viewport at the pre-defined size
	glViewport(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);


	glfwSetWindowSizeCallback(window, Callback_WindowSize);


	Texture textures[]
	{
		Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		//Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};


	BuildHeightMap();

	float scale = 0.5f;

	constexpr size_t vertexCount = resolution * resolution;
	constexpr size_t indexCount = ((resolution - 1) * (resolution - 1)) * 6;


	// UV coords.
	int u = 0;
	int v = 0;
	int increment = 20.0f / resolution;

	int vertIndex = 0;

	Vertex vertices_gen[vertexCount]{};



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

	GLuint indices_gen[indexCount]{};

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
	Shader shaderProgram("default.vert", "default.frag");
	std::vector<Vertex> vertices_vec(vertices_gen, vertices_gen + vertexCount);
	std::vector<GLuint> indices_vec(indices_gen, indices_gen + indexCount);
	std::vector<Texture> textures_vec(textures, textures + sizeof(textures) / sizeof(Texture));

	Mesh floor(vertices_vec, indices_vec, textures_vec);


	// Define the object's position
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 objectScale = glm::vec3(scale, 1.0f, scale);

	// Create an identity matrix for the object and translate it to the origin
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);
	objectModel = glm::scale(objectModel, objectScale);


	// Activate our base shader so we are using it
	shaderProgram.Activate();

	// Pass in our object's model and light colour and position to the shader
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));


	// Must enable depth test
	glEnable(GL_DEPTH_TEST);

	// Create our camera at the window's size
	Camera camera = Camera(WINDOW_SIZE_X, WINDOW_SIZE_Y, glm::vec3(12.31f, 5.58f, 12.89f));
	camera.Forward = glm::vec3(-0.656790f, -0.319373f, -0.683102f);

	// MSAA
	glEnable(GL_MULTISAMPLE);

	// IMGUI setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// External vars
	bool draw = true;
	bool wireframe = false;
	
	float colours[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float customLightPos[] = { 0.5f, 0.5f, 0.5f };


	GLuint wireframeUni = glGetUniformLocation(shaderProgram.ID, "isWireframe");


	float previousTime = glfwGetTime();

	// Main application while loop, closes when requested (i.e. press X top-right corner)
	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		float deltaTime = glfwGetTime() - previousTime;
		float FPS = 1.0f / deltaTime;

		// Set the background colour of the window to be a nice cornflower blue
		glClearColor(0.39215686274f, 0.58431372549f, 0.9294117647f, 1.0f);

		// Clear the back buffer and assign the new colour + clear the depth buffer for 3D
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// -- ImGUI frame setup 
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// -- ImGUI frame setup end


		// TODO: need a better way of doing this!
		// Ensure the camera is at the correct size
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		camera.SetCameraSize(width, height);


		// --- Core program



		// Process camera input
		camera.Inputs(window);

		// Update camera matrices
		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

		// Draw the scene using triangles and the size of the indices
		if (draw)
		{
			// Wireframe toggle
			glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

			// Define the object's position
			glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 objectScale = glm::vec3(scale, 1.0f, scale);

			// Create an identity matrix for the object and translate it to the origin
			glm::mat4 objectModel = glm::mat4(1.0f);
			objectModel = glm::translate(objectModel, objectPos);
			objectModel = glm::scale(objectModel, objectScale);

			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));

			// --------
			// -- MESHES
			floor.Draw(shaderProgram, camera);
		}



		// --- End program


		// -- ImGUI functions
		ImGui::Begin("Debug");
		ImGui::Text("%.1f FPS", FPS);

		ImGui::Checkbox("Draw?", &draw);

		// Wireframe
		ImGui::Checkbox("Wireframe Mode", &wireframe);
		glUniform1f(wireframeUni, wireframe);

		ImGui::Text("-- Camera --");
		ImGui::Text("Camera Position: %f, %f, %f", camera.Position.x, camera.Position.y, camera.Position.z);
		ImGui::Text("Camera Forward Vector: %f, %f, %f", camera.Forward.x, camera.Forward.y, camera.Forward.z);
		ImGui::Text("Camera Pitch: %f", camera.pitch);
		ImGui::Text("Camera Yaw: %f", camera.yaw);
		ImGui::Text("-- Plane --");
		ImGui::DragFloat("Scale", &scale, 0.01f, 0.01f, 10.0f);

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// -- END ImGUI


		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		// Process any native GLFW events (i.e. window resizing etc.)
		glfwPollEvents();

		previousTime = glfwGetTime();
	}

	// Shut down ImGUI and CleanUp
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Delete all the objects we have created
	shaderProgram.Delete();

	// Destroy the application's window
	glfwDestroyWindow(window);

	// Stop GLFW
	glfwTerminate();

	// End of application
	return 0;
}
