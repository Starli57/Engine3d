#pragma once

#include <glm/glm.hpp>

struct Vertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 color;

	Vertex() {};

	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color, glm::vec2 uv) 
	{
		this->position = position;
		this->normal = normal;
		this->uv = uv;
		this->color = color;
	}

	bool operator==(const Vertex& other) const 
	{
		return position == other.position 
			&& color == other.color 
			&& uv == other.uv;
	}
};