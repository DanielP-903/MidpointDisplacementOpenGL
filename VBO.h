#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glm/glm.hpp>
#include<glad/glad.h>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 colour;
	glm::vec2 UV;
};

class VBO
{
public:
	// Reference identifier for the Vertex Buffer Object
	GLuint ID;

	// Constructor for the Vertex Buffer Object, linking to the vertices given and using the size
	VBO(std::vector<Vertex>& vertices);

	// Binds the VBO
	void Bind();

	// Unbinds the VBO
	void Unbind();

	// Deletes the VBO
	void Delete();
};

#endif // !VBO_CLASS_H
