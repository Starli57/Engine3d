#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	struct Vertex 
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 uv;

		Vertex() 
		{
			position = glm::vec3(0.0f);
			normal = glm::vec3(0.0f);
			color = glm::vec3(1.0f);
			uv = glm::vec2(0.0f);
		}

		Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color, glm::vec2 uv)
		{
			this->position = position;
			this->normal = normal;
			this->color = color;
			this->uv = uv;
		}

		bool operator==(const Vertex& other) const 
		{
			return position == other.position 
				&& normal == other.normal
				&& color == other.color 
				&& uv == other.uv;
		}
	};
}