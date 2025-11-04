
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <random>
#include "Scene.h"


int WINDOW_SIZE_X = 1200;
int WINDOW_SIZE_Y = 800;


void Callback_WindowSize(GLFWwindow* window, int width, int height)
{
	WINDOW_SIZE_X = width;
	WINDOW_SIZE_Y = height;
	glViewport(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);
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

	// Must enable depth test
	glEnable(GL_DEPTH_TEST);

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

	Scene* currentScene = new Scene(window, WINDOW_SIZE_X, WINDOW_SIZE_Y);
	currentScene->PreInit();
	currentScene->Init();
	currentScene->PostInit();

	float previousTime = glfwGetTime();

	// Main application while loop, closes when requested (i.e. press X top-right corner)
	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		// Set the background colour of the window to be a nice cornflower blue
		glClearColor(0.39215686274f, 0.58431372549f, 0.9294117647f, 1.0f);

		// Clear the back buffer and assign the new colour + clear the depth buffer for 3D
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// --------------------
		// -- Main App Logic --
		// --------------------

		// Process Input for the current scene
		currentScene->Input();

		// Update the current scene
		currentScene->Update(deltaTime);

		// Render the current scene
		currentScene->Render();
		
		// -- ImGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Debug");

		// Update properties of current scene debug window
		currentScene->DebugRender();

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// -- END ImGUI

		// --------------------


		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		// Process any native GLFW events (i.e. window resizing etc.)
		glfwPollEvents();
	}

	currentScene->Shutdown();
	delete currentScene;
	currentScene = nullptr;

	// Shut down ImGUI and CleanUp
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Destroy the application's window
	glfwDestroyWindow(window);

	// Stop GLFW
	glfwTerminate();

	// End of application
	return 0;
}
