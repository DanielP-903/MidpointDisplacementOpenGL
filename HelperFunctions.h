#ifndef HELPER_FUNCTIONS_CLASS_H
#define HELPER_FUNCTIONS_CLASS_H

#include<glm/glm.hpp>

class MathHelpers
{
public:
	// Mathematical helpers

	template <typename T> 
	static inline T lerp(const T& a, const T& b, float t)
	{
		return a * (b - a) + t;
	}

	template <typename T>
	static inline float inverseLerp(const T& a, const T& b, const T& value)
	{
		return (value - a) / (b - a);
	}

	static inline glm::vec2 positionOnCircle(glm::vec2 centre, float radius, float angle)
	{
		float coords[] = { 0.0f, 0.0f };
		coords[0] = radius * glm::cos(glm::radians(angle)) + centre.x;
		coords[1] = radius * glm::sin(glm::radians(angle)) + centre.y;

		return glm::make_vec2(coords);
	}

	static inline glm::vec2 Midpoint(const glm::vec2& a, const glm::vec2& b)
	{
		return (a + b) * 0.5f;
	}
};


#endif // !HELPER_FUNCTIONS_CLASS_H
