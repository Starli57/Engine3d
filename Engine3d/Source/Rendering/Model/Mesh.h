#pragma once

#include <vector>

#include "Rendering/Model/Vertex.h"

class Mesh
{
public:
	std::vector<Vertex>* GetVertices();
	std::vector<uint32_t>* GetIndices();

	size_t GetVerticesCount();
	size_t GetIndicesCount();

	Mesh(std::vector<Vertex>* vertices, std::vector<uint32_t>* indices);
	~Mesh();

private:
	std::vector<Vertex>* vertices;
	std::vector<uint32_t>* indices;
};