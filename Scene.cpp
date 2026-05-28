#include "Scene.h"
#include "HelperFunctions.h"
#include <random>
#include <imgui.h>
#include <filesystem>
#include <cmath>
#include <utility>

// Vertices coordinates
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
	Vertex{vec3(-1.0f, 0.0f,  1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)},
	Vertex{vec3(-1.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)},
	Vertex{vec3(1.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)},
	Vertex{vec3(1.0f, 0.0f,  1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

Vertex lightVertices[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
	Vertex{vec3(-0.1f, -0.1f,  0.1f), vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)},
	Vertex{vec3(-0.1f, -0.1f, -0.1f), vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)},
	Vertex{vec3( 0.1f, -0.1f, -0.1f), vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)},
	Vertex{vec3( 0.1f, -0.1f,  0.1f), vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)},
	Vertex{vec3(-0.1f,  0.1f,  0.1f), vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)},
	Vertex{vec3(-0.1f,  0.1f, -0.1f), vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)},
	Vertex{vec3( 0.1f,  0.1f, -0.1f), vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)},
	Vertex{vec3( 0.1f,  0.1f,  0.1f), vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)}
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};


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
	//mainCamera = new Camera(screenWidth, screenHeight, vec3(-1.49f, 1.66f, 0.2));
	//mainCamera = new Camera(screenWidth, screenHeight, vec3(0.0f, 0.0f, 0.0f));
	//mainCamera = new Camera(screenWidth, screenHeight, vec3(0.0f, 1.0f, 2.0f));
	mainCamera = new Camera(screenWidth, screenHeight, vec3(12.31f, 5.58f, 12.89f));
	//mainCamera->Forward = vec3(0.899662f, -0.436409f, -0.012432f);
	//mainCamera->Forward = vec3(-0.656790f, -0.319373f, -0.683102f);

	std::string planksPath = std::string(PROJECT_DIR) + "/planks.png";
	Texture textures[]
	{
		Texture(planksPath.c_str(), "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
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
	
	float defaultScale = 1.0f;
	
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
	
			vertices_gen[vertIndex].position = vec3(posX, heightMap[vertIndex], posZ);
			vertices_gen[vertIndex].colour = vec3(1.0f, 1.0f, 1.0f);
			vertices_gen[vertIndex].normal = vec3(0.0f, 1.0f, 0.0f);
			vertices_gen[vertIndex].UV = vec2(u, v);
	
			// *
	
			u += increment;
			vertIndex++;
		}
		u = 0;
		v += increment;
	}
	
	std::unique_ptr<GLuint[]> indices_gen(new GLuint[indexCount]{});
	
	vec3 p1{}, p2{}, p3{}, A{}, B{}, normal{};
	
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
	
			p1 = vertices_gen[indices_gen[vertIndex]].position;
			p2 = vertices_gen[indices_gen[vertIndex + 1]].position;
			p3 = vertices_gen[indices_gen[vertIndex + 2]].position;
			A = p2 - p1;
			B = p3 - p1;
			
			vec3 normal = -normalize(cross(A, B));
			
			vertices_gen[indices_gen[vertIndex]].normal = normal;
			vertices_gen[indices_gen[vertIndex + 1]].normal = normal;
			vertices_gen[indices_gen[vertIndex + 2]].normal = normal;
	
			// * * * * *
			// *     *
			// *   *
			// * *
			// *
	
			indices_gen[vertIndex + 3] = (z * resolution) + x;
			indices_gen[vertIndex + 4] = (z * resolution) + (x + 1);
			indices_gen[vertIndex + 5] = ((z + 1) * resolution) + (x + 1);
			
			p1 = vertices_gen[indices_gen[vertIndex + 3]].position;
			p2 = vertices_gen[indices_gen[vertIndex + 4]].position;
			p3 = vertices_gen[indices_gen[vertIndex + 5]].position;
			A = p2 - p1;
			B = p3 - p1;
			
			normal = -normalize(cross(A, B));
			
			vertices_gen[indices_gen[vertIndex + 3]].normal = normal;
			vertices_gen[indices_gen[vertIndex + 4]].normal = normal;
			vertices_gen[indices_gen[vertIndex + 5]].normal = normal;
	
			//         *
			//       * *
			//     *   *
			//   *     *
			// * * * * *
	
			vertIndex += 6;
		}
	}

	// Create our normals display shader
	std::string normalsDisplayVertex = std::string(PROJECT_DIR) + "/normalsDisplay.vert";
	std::string normalsDisplayFragment = std::string(PROJECT_DIR) + "/normalsDisplay.frag";
	std::string normalsDisplayGeometry = std::string(PROJECT_DIR) + "/normalsDisplay.geom";
	normalsDisplayProgram = new Shader(normalsDisplayVertex.c_str(), normalsDisplayFragment.c_str(), normalsDisplayGeometry.c_str());

	// Create our shader program using our shader class and our defined default.vert and default.frag, vertex and fragment (pixel) shaders.
	std::string shaderVertex = std::string(PROJECT_DIR) + "/default.vert";
	std::string shaderFragment = std::string(PROJECT_DIR) + "/default.frag";
	shaderProgram = new Shader(shaderVertex.c_str(), shaderFragment.c_str());

	vertices_vec = std::vector<Vertex>(vertices_gen.get(), vertices_gen.get() + vertexCount);
	indices_vec = std::vector<GLuint>(indices_gen.get(), indices_gen.get() + indexCount);
	std::vector<Texture> textures_vec(textures, textures + sizeof(textures) / sizeof(Texture));
	
	//vertices_vec = std::vector<Vertex>(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	//std::vector<GLuint> indices_vec(indices, indices + sizeof(indices) / sizeof(GLuint));
	//std::vector<Texture> textures_vec(textures, textures + sizeof(textures) / sizeof(Texture));
	
	floor = new Mesh(vertices_vec, indices_vec, textures_vec);


	// Create our light shader
	std::string lightVertex = std::string(PROJECT_DIR) + "/light.vert";
	std::string lightFragment = std::string(PROJECT_DIR) + "/light.frag";
	lightProgram = new Shader(lightVertex.c_str(), lightFragment.c_str());

	// Create an identity matrix for the light cube and translate it to the light's position
	mat4 lightModel = mat4(1.0f);
	lightModel = translate(lightModel, lightPos);

	std::vector<Vertex> lightVertices_vec(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector<GLuint> lightIndices_vec(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));

	light = new Mesh(lightVertices_vec, lightIndices_vec, std::vector<Texture>());


	// Define the object's position
	vec3 objectPos = vec3(0.0f, 0.0f, 0.0f);
	vec3 objectSize = vec3(objectScale, 1.0f, objectScale);

	// Create an identity matrix for the object and translate it to the origin
	mat4 objectModel = mat4(1.0f);
	objectModel = translate(objectModel, objectPos);
	objectModel = scale(objectModel, objectSize);


	// Activate our base shader so we are using it
	shaderProgram->Activate();

	// Pass in our object's model and light colour and position to the shader
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "model"), 1, GL_FALSE, value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	wireframeUni = glGetUniformLocation(shaderProgram->ID, "isWireframe");

	normalsDisplayProgram->Activate();

	glUniformMatrix4fv(glGetUniformLocation(normalsDisplayProgram->ID, "model"), 1, GL_FALSE, value_ptr(objectModel));

	testEvent += Bind(this, &Scene::TestFunc);
}

void Scene::TestFunc() 
{
	std::cout << "Event fired!" << "\n";
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

	// Update shader uniforms
	glUniform1f(glGetUniformLocation(shaderProgram->ID, "time"), elapsedTime);
	glUniform1f(glGetUniformLocation(shaderProgram->ID, "textureScale"), texScale);
	glUniform1f(glGetUniformLocation(shaderProgram->ID, "freq"), frequency);
	glUniform1f(glGetUniformLocation(shaderProgram->ID, "amp"), amplitude);
	glUniform1f(glGetUniformLocation(shaderProgram->ID, "speed"), speed);

	// Update light colour and position
	lightColor = make_vec4(colours);
	lightPos = make_vec3(customLightPos);

	normalsDisplayProgram->Activate();

	glUniform1f(glGetUniformLocation(normalsDisplayProgram->ID, "MAGNITUDE"), mag);


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
	vec3 objectPos = vec3(0.0f, 0.0f, 0.0f);
	vec3 objectSize = vec3(objectScale, 1.0f, objectScale);

	// Create an identity matrix for the object and translate it to the origin
	mat4 objectModel = mat4(1.0f);
	objectModel = translate(objectModel, objectPos);
	objectModel = scale(objectModel, objectSize);

	shaderProgram->Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "model"), 1, GL_FALSE, value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	vec3 sunDirection = make_vec3(directionalDir);
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "directionalDir"), sunDirection.x, sunDirection.y, sunDirection.z);

	glUniform1f(glGetUniformLocation(shaderProgram->ID, "attenuationQuadratic"), attenuationQuadraticValue);
	glUniform1f(glGetUniformLocation(shaderProgram->ID, "attenuationLinear"), attenuationLinearValue);
	glUniform1f(glGetUniformLocation(shaderProgram->ID, "attenuationConstant"), attenuationConstantValue);

	// Activate and use our light shader
	lightProgram->Activate();

	// Create an identity matrix for the light cube and translate it to the light's position
	mat4 lightModel = mat4(1.0f);
	lightModel = translate(lightModel, lightPos);

	// Update light shader uniforms
	glUniformMatrix4fv(glGetUniformLocation(lightProgram->ID, "model"), 1, GL_FALSE, value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightProgram->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);


	// --------
	// -- MESHES
	floor->Draw(*shaderProgram, *mainCamera);
	light->Draw(*lightProgram, *mainCamera);

	normalsDisplayProgram->Activate();

	// then draw model with normal visualizing geometry shader
	glUniformMatrix4fv(glGetUniformLocation(normalsDisplayProgram->ID, "model"), 1, GL_FALSE, value_ptr(objectModel));

	floor->Draw(*normalsDisplayProgram, *mainCamera);

}

void Scene::DebugRender()
{
	//std::cout << "Scene Debug Render" << "\n";

	ImGui::Text("-- Debugging --");
	ImGui::Text("%.1f FPS", FPS);
	ImGui::Checkbox("Draw?", &draw);
	ImGui::Checkbox("Wireframe Mode", &wireframe);
	ImGui::InputFloat("Normal Visualisers Size", &mag, 0.5f);
	ImGui::Text("-- Camera --");
	ImGui::Text("Camera Position: %f, %f, %f", mainCamera->Position.x, mainCamera->Position.y, mainCamera->Position.z);
	ImGui::Text("Camera Forward Vector: %f, %f, %f", mainCamera->Forward.x, mainCamera->Forward.y, mainCamera->Forward.z);
	ImGui::Text("Camera Pitch: %f", mainCamera->pitch);
	ImGui::Text("Camera Yaw: %f", mainCamera->yaw);
	ImGui::Text("-- Lighting --");
	ImGui::DragFloat3("Light Position:", customLightPos, 0.01f);
	ImGui::ColorEdit4("Light Colour:", colours);
	ImGui::SliderFloat3("Light Direction", directionalDir, -1.0f, 1.0f);
	ImGui::InputFloat("Light Quadratic Value", &attenuationQuadraticValue, 0.01f);
	ImGui::InputFloat("Light Linear Value", &attenuationLinearValue, 0.01f);
	ImGui::InputFloat("Light Constant Value", &attenuationConstantValue, 0.01f);
	ImGui::Text("-- Plane --");
	ImGui::DragFloat("Texture Scale", &texScale, 0.01f, 0.01f, 10.0f);
	ImGui::DragFloat("Scale", &objectScale, 0.01f, 0.01f, 10.0f);
	ImGui::DragFloat("Frequency", &frequency, 0.01f, 0.01f, 10.0f);
	ImGui::DragFloat("Amplitude", &amplitude, 0.01f, 0.01f, 10.0f);
	ImGui::DragFloat("Speed", &speed, 0.01f, 0.01f, 10.0f);
	ImGui::DragFloat("Randomness", &randomness, 0.01f, 0.01f, 10.0f);
	ImGui::Text("-- Midpoint Displacement --");
	ImGui::InputFloat("Corner Min Start Height", &SminRange, 0.25f);
	ImGui::InputFloat("Corner Max Start Height", &SmaxRange, 0.25f);
	if (ImGui::Button("Regenerate Height Map")) 
	{
		GenerateMidpointDisplacement();
	}
	if (ImGui::Button("Call Test Event")) 
	{
		testEvent.Invoke();
	}
}

void Scene::GenerateMidpointDisplacement()
{
	MidpointDisplacement();

	RecalculatePositions();
	RecalculateNormals();
	floor->Regenerate(vertices_vec);
}

void Scene::RecalculatePositions() 
{
	for (size_t i = 0; i < vertices_vec.size(); i++)
	{
		vertices_vec[i].position = vec3(
			vertices_vec[i].position.x,
			heightMap[i], 
			vertices_vec[i].position.z
		);
	}
}

void Scene::RecalculateNormals() 
{
	vec3 p1{}, p2{}, p3{}, A{}, B{}, normal{};

	// Re-calculate normals
	for (size_t i = 0; i < vertices_vec.size(); i += 3)
	{
		// *
		// * *
		// *   *
		// *     *
		// * * * * *

		p1 = vertices_vec[indices_vec[i]].position;
		p2 = vertices_vec[indices_vec[i + 1]].position;
		p3 = vertices_vec[indices_vec[i + 2]].position;
		A = p2 - p1;
		B = p3 - p1;

		normal = -normalize(cross(A, B));

		vertices_vec[indices_vec[i]].normal = normal;
		vertices_vec[indices_vec[i + 1]].normal = normal;
		vertices_vec[indices_vec[i + 2]].normal = normal;
	}
}

// Midpoint displacement algorithm for generating terrain
void Scene::MidpointDisplacement(int startValue, bool useDefault)
{
	// Use the modified min and max ranges
	minRange = SminRange;
	maxRange = SmaxRange;

	// Get the size of terrain iterations required
	int resolutionSize = abs(sqrt(resolution - 1));

	// Ensure that resolution is correct size (2^n + 1)
	for (int i = 0; i < 12; i++)
	{
		// Check resolution
		if (pow(2, i) == (resolution - 1))
		{
			// Increase size based on resolution
			resolutionSize = i + 1;
			break;
		}
	}

	// Initialise grid (default height)
	for (size_t i = 0; i < resolution * resolution; i++)
	{
		heightMap[i] = 0.0f;
	}

	// Seed corners initially
	SetHeight(ivec2(0, 0), useDefault ? GenerateRandomHeight(maxRange) : customCornerValues[0]);
	SetHeight(ivec2(0, resolution - 1), useDefault ? GenerateRandomHeight(maxRange) : customCornerValues[1]);
	SetHeight(ivec2(resolution - 1, 0), useDefault ? GenerateRandomHeight(maxRange) : customCornerValues[2]);
	SetHeight(ivec2(resolution - 1, resolution - 1), useDefault ? GenerateRandomHeight(maxRange) : customCornerValues[3]);

	// Initial square end point, initially terrain resolution
	int squareSize = resolution - 1;

	// Initialise square counts
	int noSquares = (resolution - 1) / squareSize;
	int noSquaresHorizontal = 1;
	int noSquaresVertical = 1;
	
	ivec2 squareStart = { 0, 0 };
	ivec2 squareEnd = { 0, 0 };

	int itr = 0;

	// Do midpoint displacement until the number of squares reaches maximum
	while (itr < resolutionSize)
	{
		// Get no of squares at this iteration
		noSquares = pow(4, itr);

		itr++;

		squareStart = { 0, 0 };
		squareEnd = { 0, 0 };

		squareSize = (resolution - 1) / sqrt(noSquares);
		noSquaresHorizontal = sqrt(noSquares);
		noSquaresVertical = sqrt(noSquares);

		// Horizontal squares
		for (int i = 0; i < noSquaresHorizontal; i++)
		{
			squareEnd.x += squareSize;
			squareEnd.y = 0;
			squareStart.y = 0;

			// Check if any pos has gone beyond terrain limits
			if (squareEnd.x > resolution || squareStart.x > resolution)
				break;

			// Vertical squares
			for (int j = 0; j < noSquaresVertical; j++)
			{
				squareEnd.y += squareSize;

				// Check if any pos has gone beyond terrain limits
				if (squareEnd.y > resolution || squareStart.y > resolution)
					break;

				// Find corners, centre and sides of this square at the start x,y and end x,y
				SquareStep(squareStart, squareEnd);
				DiamondStep(squareStart, squareEnd);

				squareStart.y += squareSize;
			}

			squareStart.x += squareSize;
		}

		minRange *= 0.5f;
		maxRange *= 0.5f;
	}
}

// Square step to work out the corner heights and centre of each square
// Subsidiary of the midpoint displacement algorithm
void Scene::SquareStep(ivec2 start, ivec2 end)
{
	// Before
	// ? --------- ?
	// |           |
	// |           |
	// |           |
	// |           |
	// |           |
	// ? --------- ?

	// Corner positions
	ivec2 topLeft = start;
	ivec2 bottomLeft = ivec2(start.x, end.y);
	ivec2 topRight = ivec2(end.x, start.y);
	ivec2 bottomRight = end;

	// Get corner values of the square
	cornerValues[0] = GetHeight(topLeft);
	cornerValues[1] = GetHeight(bottomLeft);
	cornerValues[2] = GetHeight(topRight);
	cornerValues[3] = GetHeight(bottomRight);

	// Calculate centre point
	ivec2 centre = MathHelpers::Midpoint(start, end);

	// Calculate height
	float centreHeight = (cornerValues[0] + cornerValues[1] + cornerValues[2] + cornerValues[3]) / (sizeof(cornerValues) / sizeof(int));

	// Finally assign the height
	SetHeight(centre, centreHeight);

	// After
	// 0 --------- 2
	// |     |     |
	// |     |     |
	// | --- C --- |
	// |     |     |
	// |     |     |
	// 1 --------- 3
}

// Diamond step to work out side heights of each square
// Subsidiary of the midpoint displacement algorithm
void Scene::DiamondStep(ivec2 start, ivec2 end)
{
	// Calculate height of midpoint by getting the averages of the corner values
	
	// Before
	// 0 --------- 2
	// |           |
	// |           |
	// |           |
	// |           |
	// |           |
	// 1 --------- 3

	// Corner positions
	ivec2 topLeft		= start;
	ivec2 bottomLeft	= ivec2(start.x, end.y);
	ivec2 topRight		= ivec2(end.x, start.y);
	ivec2 bottomRight	= end;

	// Get each side's midpoint
	ivec2 left		= MathHelpers::Midpoint(topLeft, bottomLeft);
	ivec2 right		= MathHelpers::Midpoint(topRight, bottomRight);
	ivec2 top		= MathHelpers::Midpoint(topLeft, topRight);
	ivec2 bottom	= MathHelpers::Midpoint(bottomLeft, bottomRight);

	// Get a randomised average height at each side
	float leftHeight	= RandomAverage(cornerValues[0], cornerValues[1]);
	float rightHeight	= RandomAverage(cornerValues[2], cornerValues[3]);
	float topHeight		= RandomAverage(cornerValues[0], cornerValues[2]);
	float bottomHeight	= RandomAverage(cornerValues[1], cornerValues[3]);
	
	// Finally assign the calculated points and heights
	SetHeight(left, leftHeight);
	SetHeight(right, rightHeight);
	SetHeight(top, topHeight);
	SetHeight(bottom, bottomHeight);

	// After
	// 0 --- T --- 2
	// |   *   *   |
	// | *       * |
	// L           R
	// | *       * |
	// |   *   *   |
	// 1 --- B --- 3
}

float Scene::RandomAverage(float a, float b)
{
	// Calculate random value based on max and min terrain height range
	float random = ((float)rand() / (float)RAND_MAX) * (maxRange - minRange) + minRange;

	return ((a + b) * 0.5f) + random;
}

// Function for generating a random height value based on a maximum value
float Scene::GenerateRandomHeight(float maxHeight)
{
	float minValue = maxHeight - (maxHeight * 2.0f);
	return ((float)rand() / (float)RAND_MAX) * (maxHeight - minValue) + minValue;
}

float Scene::GetHeight(ivec2 position)
{
	return heightMap[(position.x * resolution) + position.y];
}

float Scene::GetHeight(int x, int y)
{
	return heightMap[(x * resolution) + y];
}

void Scene::SetHeight(ivec2 position, float height)
{
	heightMap[(position.x * resolution) + position.y] = height;
}




















void Scene::Shutdown()
{
	std::cout << "Scene Shutdown" << "\n";

	// Delete all the objects we have created
	shaderProgram->Delete();

	normalsDisplayProgram->Delete();

	lightProgram->Delete();

	delete shaderProgram;
	shaderProgram = nullptr;

	delete normalsDisplayProgram;
	normalsDisplayProgram = nullptr;

	delete lightProgram;
	lightProgram = nullptr;

	delete mainCamera;
	mainCamera = nullptr;

	delete floor;
	floor = nullptr;

	delete light;
	light = nullptr;
}
