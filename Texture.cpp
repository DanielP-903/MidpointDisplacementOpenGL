#include "Texture.h"

// Constructs the texture
Texture::Texture(const char* image, const char* textureType, GLuint slot, GLenum format, GLenum pixelType)
{
	type = textureType;
	
	// Using the STBI lib load the requested image
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);	// Must flip vertically as OpenGL reads textures inversely in comparison to STBI
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);	// 0, NOT "STBI_rgb_alpha" !!!!

	// -- TEXTURE INIT 
	
	// Reserve memory for texture
	glGenTextures(1, &ID);

	// Set the active texture slot (e.g. GL_TEXTURE0, GL_TEXTURE1 etc.)
	glActiveTexture(GL_TEXTURE0 + slot);

	// Store our texture memory space
	unit = slot;

	// Binds the texture to the memory we reserved
	glBindTexture(GL_TEXTURE_2D, ID);

	// Set the texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set the texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Defines the texture using the provided data and custom params
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);

	// Create mipmaps for the active texture
	glGenerateMipmap(GL_TEXTURE_2D);

	// -- INIT END

	// Free up our image and clear the bound texture
	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Create the texture unit using a specified shader
void Texture::textureUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}

// Binds the texture
void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

// Unbinds the texture
void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, NULL);
}

// Deletes the texture
void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}