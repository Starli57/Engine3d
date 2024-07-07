#pragma once

#include <vector>
#include <filesystem>

#include "EngineShared/Ref.h"
#include "EngineShared/Vertex.h"

class Mesh
{
public:
	Ref<std::vector<Vertex>> GetVertices();
	Ref<std::vector<uint32_t>> GetIndices();

	size_t GetVerticesCount();
	size_t GetIndicesCount();

	Mesh(const std::filesystem::path& path);
	Mesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices);
	virtual ~Mesh();

protected:
	Ref<std::vector<Vertex>> vertices;
	Ref<std::vector<uint32_t>> indices;
};