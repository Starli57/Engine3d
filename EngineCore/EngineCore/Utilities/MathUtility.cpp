#include "EngineCore/Pch.h"
#include "MathUtility.h"

void CalculateDirection(glm::vec3* direction, const glm::vec3& rotation)
{
	direction->x = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
	direction->y = sin(glm::radians(rotation.y));
	direction->z = sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
}

void Normalize(glm::vec3* vector)
{
	*vector = normalize(*vector);
}