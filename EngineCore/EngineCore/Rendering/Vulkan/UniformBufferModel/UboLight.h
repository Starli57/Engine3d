#pragma once

#include <glm/glm.hpp>

namespace VulkanApi
{
	struct UboLight
	{
		glm::mat4 viewProjection;
		glm::vec4 colorAndIntensity;
		glm::vec3 position;
		glm::vec3 direction;

		UboLight(const glm::mat4& viewProjection, const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float intensity) : 
			viewProjection(viewProjection), position(position), direction(direction), colorAndIntensity(glm::vec4(color.r, color.g, color.b, intensity)) {}
	};
}