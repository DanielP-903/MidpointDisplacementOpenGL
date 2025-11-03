#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:
	// Reference identifier for the Vertex Array Object
	GLuint ID;

	// Constructor for the Vertex Array Object, generates a VAO ID
	VAO();

	// Links this VAO with a given VBO and layout
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, void* offset);

	// Binds the VAO
	void Bind();

	// Unbinds the VAO
	void Unbind();
	
	// Deletes the VAO
	void Delete();
};

#endif // !VAO_CLASS_H
