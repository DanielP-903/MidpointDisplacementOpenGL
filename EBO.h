#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glm/glm.hpp>
#include<glad/glad.h>
#include <vector>

class EBO
{
public:
	// Reference identifier for the Element Buffer Object
	GLuint ID;

	// Constructor for the Element Buffer Object, linking to the indices given and using the size
	EBO(std::vector<GLuint>& indices);

	// Binds the EBO
	void Bind();

	// Unbinds the EBO
	void Unbind();

	// Deletes the EBO
	void Delete();
};

#endif // !EBO_CLASS_H
