#pragma once

#include <glm/glm.hpp>

namespace AVulkan
{
	struct UboLight
	{
		glm::mat4 viewProjection;
		glm::vec3 position;
		glm::vec3 direction;

		UboLight(glm::mat4& viewProjection, glm::vec3& position, glm::vec3& direction) : 
			viewProjection(viewProjection), position(position), direction(direction) {};
	};
}