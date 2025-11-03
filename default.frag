#version 330 core	// Required to tell OpenGL we are using version 3.3

// Outputs a fragment colour
out vec4 FragColor;


// Inputs current object position
in vec3 currentPosition;

// Inputs normal vector
in vec3 Normal;

// Inputs the color vector calculated using the vertex shader
in vec3 color;

// Inputs texture coordinate
in vec2 texCoord;


// Is Wireframe
uniform bool isWireframe;

// Samplers for the textures we are using
uniform sampler2D diffuse0;
uniform sampler2D specular0;

// Uniform vars for light vars and camera position
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

// Light type enums
const uint Directional  = 0x00000001u;
const uint Point        = 0x00000002u;
const uint Spot			= 0x00000004u;
uniform uint lightType; 

// Light type uniform vars

// Directional Light (Sun)
uniform vec3 directionalDir = vec3(1.0f,1.0f, 0.0f);

// Attenuation (Point Light)
uniform float attenuationQuadratic = 3.0f;
uniform float attenuationLinear = 0.7f;
uniform float attenuationConstant = 1.0f;	// Keep at 1.0f or might result in boosting it too much

// Spotlight Angles
uniform vec3 spotlightDirection = vec3(0.0f,-1.0f, 0.0f);
uniform float spotlightInnerAngle = 0.95f;
uniform float spotlightOuterAngle = 0.9f;

// Default directional light (e.g. the Sun)
vec4 directionalLight()
{
	// Ambient light value
	float ambient = 0.2f;


	// Get our light direction based on the normal
	vec3 normal = normalize(Normal);
	vec3 lightVec = lightPos - currentPosition;
	vec3 lightDirection = normalize(directionalDir);


	// Calculate diffuse lighting value (fade off from normal and light direction
	float diffuse = max(dot(normal, lightDirection), 0.0f);


	// Calculate specular highlights
	float specularLight = 0.5f;
	vec3 viewDir = normalize(camPos - currentPosition);
	vec3 reflectionDir = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), 16);
	float specular = specAmount * specularLight;
	

	// Ouput our calculate light shading on texture
	return (texture(diffuse0, vec2(texCoord.x,texCoord.y)) * (diffuse + ambient) + texture(specular0, vec2(texCoord.x, texCoord.y)).r * specular) * lightColor;
}

// A basic point light with fall off
vec4 pointLight()
{	
	// Ambient light value
	float ambient = 0.2f;


	// Falloff (attenuation)
	vec3 lightVec = lightPos - currentPosition;
	float dist = length(lightVec);

	// Calculate attentuation
	float intensity = 1.0f / (attenuationQuadratic * dist * dist + attenuationLinear * dist + attenuationConstant);


	// Get our light direction based on the normal
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);


	// Calculate diffuse lighting value (fade off from normal and light direction
	float diffuse = max(dot(normal, lightDirection), 0.0f);


	// Calculate specular highlights
	float specularLight = 0.5f;
	vec3 viewDir = normalize(camPos - currentPosition);
	vec3 reflectionDir = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), 16);
	float specular = specAmount * specularLight;
	

	// Ouput our calculate light shading on texture
	return (texture(diffuse0, vec2(texCoord.x,texCoord.y)) * (diffuse * intensity + ambient) + texture(specular0, vec2(texCoord.x, texCoord.y)).r * specular * intensity) * lightColor;
}

// A spot light which uses an inner and outer cone to create the effect (e.g. a flashlight)
vec4 spotLight()
{
	// Ambient light value
	float ambient = 0.2f;


	// Get our light direction based on the normal
	vec3 normal = normalize(Normal);
	vec3 lightVec = lightPos - currentPosition;
	vec3 lightDirection = normalize(lightVec);


	// Dot a downwards direction with the spotlightDirection to get an angle for the cone
	float theta = dot(spotlightDirection, -lightDirection);

	// Using the inner and outer angles for the cone calculations,
	// calculate the cutoff amount to create a smooth gradient from inner to outer cones
	float cutOff = clamp((theta - spotlightOuterAngle) / (spotlightInnerAngle - spotlightOuterAngle), 0.0f, 1.0f); 


	// Calculate diffuse lighting value (fade off from normal and light direction
	float diffuse = max(dot(normal, lightDirection), 0.0f);


	// Calculate specular highlights
	float specularLight = 0.5f;
	vec3 viewDir = normalize(camPos - currentPosition);
	vec3 reflectionDir = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0f), 16);
	float specular = specAmount * specularLight;
	

	// Ouput our calculate light shading on texture
	return (texture(diffuse0, texCoord) * (diffuse * cutOff + ambient) + texture(specular0, texCoord).r * specular * cutOff) * lightColor;
}

void main()
{
	FragColor = texture(diffuse0, texCoord);
	return;

	// --
	// For debug wireframe mode
	if (isWireframe)
	{
		FragColor = lightColor; //* (diffuse + ambient + specular);
		return;
	}
	// --

	vec4 result;

	// Apply texture and light info to fragment colour based on light type
	switch (lightType)
	{
		case Directional:	result = directionalLight();	break;		// Directional Light
		case Point:			result = pointLight();			break;		// Point Light
		case Spot:			result = spotLight();			break;		// Spot Light
		default:			result = lightColor;			break;		// Default fallback
	}

	// Output the resulting colour
	FragColor = result;
}
