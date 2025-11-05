#version 330 core	// Required to tell OpenGL we are using version 3.3

layout (location = 0) in vec3 aPos;			// Using the 1st buffer location (first 3 bytes) as position info
layout (location = 1) in vec3 aNormal;		// Using the 2nd buffer location (next 3 bytes) as normal info
layout (location = 2) in vec3 aColor;		// Using the 3rd buffer location (next 3 bytes) as colour info
layout (location = 3) in vec2 aTexCoord;	// Using the 4th buffer location (next 2 bytes) as texture coordinate info

// Outputs current position of the object for the fragment shader
out vec3 currentPosition;

// Outputs the normal vector
out vec3 Normal;

// Outputs a colour vector calculated in here
out vec3 color;

// Outputs a texture coordinate for the fragment shader
out vec2 texCoord;

// Matrix and model defined by our camera calculations in the main file
uniform mat4 camMatrix;
uniform mat4 model;

uniform float time;
uniform float freq;
uniform float amp;
uniform float speed;


void main()
{

	// Set our position to the given matrix
	currentPosition = vec3(model * vec4(aPos, 1.0f));

	currentPosition.y += (amp * sin(currentPosition.x * freq + (time * speed)) + amp * cos(currentPosition.z * freq + (time * speed)));

	// Outputs the normal vector for this vertex
	Normal = aNormal;

	// Outputs desired colour data to this vertex
	color = aColor;
	
	//color = mix(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), (currentPosition.y) / amp );

	// Outputs texture coordinates to be used
	texCoord = aTexCoord;


	// Our position using 3D matrices
	gl_Position = camMatrix * vec4(currentPosition, 1.0f);
}