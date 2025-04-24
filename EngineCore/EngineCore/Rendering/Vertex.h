#pragma once

#include <glm/glm.hpp>

struct Vertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	glm::vec3 color;
	glm::vec2 uv;

	Vertex() 
	{
	}

	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 tangent, glm::vec3 bitangent, glm::vec3 color, glm::vec2 uv)
	{
		this->position = position;
		this->normal = normal;
		this->tangent = tangent;
		this->bitangent = bitangent;
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