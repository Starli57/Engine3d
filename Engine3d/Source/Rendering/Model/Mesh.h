#pragma once

#include <vector>

#include "Macroses/Ref.h"
#include "Rendering/Model/Vertex.h"

class Mesh
{
public:
	Ref<std::vector<Vertex>> GetVertices();
	Ref<std::vector<uint32_t>> GetIndices();

	size_t GetVerticesCount();
	size_t GetIndicesCount();

	Mesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices);
	~Mesh();

private:
	Ref<std::vector<Vertex>> vertices;
	Ref<std::vector<uint32_t>> indices;
};