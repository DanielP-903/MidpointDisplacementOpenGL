#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

// Standard shader program class
class Shader
{
public:
	// Unsigned integer identifier for the shader program
	GLuint ID;

	// Constructor taking in the vertex and fragment shader files for reading
	Shader(const char* vertexFile, const char* fragmentFile);

	// Activates the shader program
	void Activate();

	// Deletes the shader program
	void Delete();

	// Compiles shader and spits out errors
	void compileErrors(unsigned int shader, const char* type);
};

#endif