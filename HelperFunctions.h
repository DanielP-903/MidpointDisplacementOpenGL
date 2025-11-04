#ifndef HELPER_FUNCTIONS_CLASS_H
#define HELPER_FUNCTIONS_CLASS_H

#include<glm/glm.hpp>

static class HelperFunctions
{
public:
	// Mathematical helpers
	static float lerp(float x, float y, float t);
	static glm::vec3 lerp(glm::vec3 x, glm::vec3 y, float t);
	static glm::vec2 positionOnCircle(glm::vec2 centre, float radius = 5.0f, float angle = 0.0f);
};


#endif // !HELPER_FUNCTIONS_CLASS_H
