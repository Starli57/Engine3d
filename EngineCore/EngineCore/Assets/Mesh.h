#pragma once

#include <vector>
#include <filesystem>

#include "Asset.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vertex.h"

class Mesh : public Asset
{
public:
	Ref<std::vector<Vertex>> GetVertices();
	Ref<std::vector<uint32_t>> GetIndices();

	size_t GetVerticesCount();
	size_t GetIndicesCount();

	Mesh(const std::filesystem::path& path);
	Mesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices);
	virtual ~Mesh();

	void LoadAndInitObj(const std::filesystem::path& path);
	void LoadAndInitGltf(const std::filesystem::path& path);

protected:
	Ref<std::vector<Vertex>> vertices;
	Ref<std::vector<uint32_t>> indices;
};