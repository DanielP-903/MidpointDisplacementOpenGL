#version 330 core	// Required to tell OpenGL we are using version 3.3

layout (location = 0) in vec3 aPos;		// Using the 1st buffer location (first 3 bytes) as position info

// Model and matrix defined by our camera calculations in the main file
uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
	// Our position using 3D matrices
   gl_Position = camMatrix * model * vec4(aPos, 1.0f);
}