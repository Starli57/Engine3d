#pragma once

#include <glm/glm.hpp>

struct Vertex 
{
	glm::vec3 position;
	glm::vec3 color;

	Vertex(glm::vec3 position, glm::vec3 color) 
	{
		this->position = position;
		this->color = color;
	}
};