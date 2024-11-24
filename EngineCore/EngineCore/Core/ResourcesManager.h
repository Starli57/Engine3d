#pragma once

#include "AssetsDatabase.h"
#include "EngineCore/Assets/Meshes/MeshDefinition.h"
#include "EngineCore/Defines/DllDefines.h"
#include "EngineCore/Rendering/IGraphicsApi.h"
#include "EngineCore/Core/Ref.h"

class ResourcesManager
{
public:
	ResourcesManager(IGraphicsApi* graphicsApi, Ref<AssetsDatabase> assetsDatabase);
	~ResourcesManager();

	int32_t LoadTextureByName(const std::string& fileName);
	int32_t LoadTextureByPath(const std::filesystem::path& path);

	int32_t LoadMeshByName(const std::string& fileName);
	int32_t LoadMeshByPath(const std::filesystem::path& path);

	int32_t CreateMesh(MeshDefinition& meshDefinition);
	int32_t CreateMesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices);

	int32_t CreateMaterial(const std::string& pipelineId);
	int32_t CreateMaterial(const std::string& pipelineId, int32_t albedoTextureIndex);

private:
	IGraphicsApi* graphicsApi;
	Ref<AssetsDatabase> assetsDatabase;
};

