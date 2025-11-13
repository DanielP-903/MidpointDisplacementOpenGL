#include "shaderClass.h"

// Helper func: gets a file name and returns it's contents for use in loading shaders
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in) 
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile)
{
	// Get the contents of the vertex and fragment shader files
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert the code we have obtained into character strings (so OpenGL can read it)
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();


	// --- Create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach vertex shader source to vertex shader object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Precompile the shader to ensure compiler knows what we're talking about
	glCompileShader(vertexShader);

	// Check for errors, are we good?
	compileErrors(vertexShader, "VERTEX");


	// --- Create fragment (pixel) shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach fragment shader source to fragment shader object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Precompile the shader to ensure compiler knows what we're talking about
	glCompileShader(fragmentShader);

	// Check for errors, are we good?
	compileErrors(fragmentShader, "FRAGMENT (PIXEL)");


	// Create the shader program
	ID = glCreateProgram();
	// Attach both vertex and fragment shaders to program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	bool usingGeo = geometryFile != "";
	GLuint geometryShader;

	// --- Create geometry shader (if provided)
	if (usingGeo)
	{
		// Get the contents of the geometry shader file
		std::string geometryCode = get_file_contents(geometryFile);

		// Convert the code we have obtained into character strings (so OpenGL can read it)
		const char* geometrySource = geometryCode.c_str();

		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		// Attach geometry shader source to geometry shader object
		glShaderSource(geometryShader, 1, &geometrySource, NULL);
		// Precompile the shader to ensure compiler knows what we're talking about
		glCompileShader(geometryShader);

		// Check for errors, are we good?
		compileErrors(geometryShader, "GEOMETRY");

		// Attach geometry shader to program
		glAttachShader(ID, geometryShader);
	}


	// Link up into created shader program
	glLinkProgram(ID);

	// Check for errors, are we good?
	compileErrors(ID, "PROGRAM");

	// No need for these anymore, discard them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	if (usingGeo)
		glDeleteShader(geometryShader);
}

// Activates the shader program
void Shader::Activate() 
{
	glUseProgram(ID);
}

// Deletes the shader program
void Shader::Delete() 
{
	glDeleteProgram(ID);
}

// Check for errors from a compiled shader and output any issues
void Shader::compileErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) 
		{
			// Detected a shader compilation fault
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for: " << type << "\n" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			// Detected a program creation fault when linking the shaders
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for: " << type << "\n" << std::endl;
		}
	}
}