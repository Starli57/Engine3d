#include "EngineCore/Pch.h"
#include "MathUtility.h"

namespace Engine
{
	void CalculateDirection(glm::vec3* direction, const glm::vec3& rotation)
	{
		direction->x = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
		direction->y = sin(glm::radians(rotation.y));
		direction->z = sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));

		direction->y = glm::clamp(direction->y, -90.0f, 90.0f);
	}

	void Normalize(glm::vec3* vector)
	{
		*vector = normalize(*vector);
	}
}