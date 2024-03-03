#pragma once

#include <glm/glm.hpp>

struct Vertex 
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 uv;

	Vertex() {};

	Vertex(glm::vec3 position, glm::vec3 color, glm::vec2 uv) 
	{
		this->position = position;
		this->color = color;
		this->uv = uv;
	}

	bool operator==(const Vertex& other) const 
	{
		return position == other.position 
			&& color == other.color 
			&& uv == other.uv;
	}
};