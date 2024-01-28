#pragma once

#include <vector>

#include "Vertex.h"

class Mesh
{
public:
	Mesh(std::vector<Vertex>* vertices, std::vector<uint32_t>* indices);
	~Mesh();

	std::vector<Vertex>* GetVertices();
	std::vector<uint32_t>* GetIndices();

	uint32_t GetVerticesCount();
	uint32_t GetIndicesCount();

private:
	std::vector<Vertex>* vertices;
	std::vector<uint32_t>* indices;
};