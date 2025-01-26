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
		this->position = glm::vec3(0, 0, 0);
		this->normal = glm::vec3(0, 0, 0);
		this->tangent = glm::vec3(0, 0, 0);
		this->bitangent = glm::vec3(0, 0, 0);
		this->color = glm::vec3(0, 0, 0);
		this->uv = glm::vec2(0, 0);
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