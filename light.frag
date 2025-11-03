#version 330 core	// Required to tell OpenGL we are using version 3.3

// Outputs a fragment colour
out vec4 FragColor;

// Uniform var for light colour information
uniform vec4 lightColor;

void main()
{	
	// Set's the pixel colour to that of the light amount
	FragColor = lightColor;
}