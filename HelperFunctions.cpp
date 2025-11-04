#include "HelperFunctions.h"
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>


// REFERENCE: https://graphicscompendium.com/opengl/22-interpolation
float HelperFunctions::lerp(float x, float y, float t)
{
	return x * (1.f - t) + y * t;
}

glm::vec3 HelperFunctions::lerp(glm::vec3 x, glm::vec3 y, float t)
{
	return x * (1.f - t) + y * t;
}

// Helper to get a position on a circle given a centre point, radius and angle
glm::vec2 HelperFunctions::positionOnCircle(glm::vec2 centre, float radius, float angle)
{
	float coords[] = { 0.0f, 0.0f };
	coords[0] = radius * glm::cos(glm::radians(angle)) + centre.x;
	coords[1] = radius * glm::sin(glm::radians(angle)) + centre.y;

	return glm::make_vec2(coords);
}
