#pragma once

#include <vector>

#include "Vertex.h"

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices);
	~Mesh();

	std::vector<Vertex> GetVertices();
	uint32_t GetVerticesCount();

private:
	std::vector<Vertex> vertices;
};