#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>

#include "shaderClass.h"

class Texture 
{
public:
	// Texture reference ID
	GLuint ID;
	
	// Texture type
	const char* type;

	// Texture unit memory space
	GLuint unit;

	// Constructs the texture
	Texture(const char* image, const char* textureType, GLuint slot, GLenum format, GLenum pixelType);

	// Create the texture unit using a specified shader
	void textureUnit(Shader& shader, const char* uniform, GLuint unit);

	// Binds the texture
	void Bind();

	// Unbinds the texture
	void Unbind();

	// Deletes the texture
	void Delete();
};

#endif
