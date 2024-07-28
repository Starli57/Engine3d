#pragma once

#include "AssetsDatabase.h"
#include "EngineShared/Texture.h"
#include "EngineShared/Material.h"
#include "EngineShared/Mesh.h"
#include "EngineShared/Ref.h"
#include "EngineShared/EventHandler.h"

class IGraphicsApi
{
public:
    virtual ~IGraphicsApi() = default;

    virtual void Init() = 0;
    virtual void Render() = 0;
    virtual void FinanilizeRenderOperations() = 0;

    virtual Ref<Mesh> CreateMesh(const std::filesystem::path& meshPath) = 0;
    virtual Ref<Mesh> CreateMesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices) = 0;
    virtual Ref<Texture> CreateTexture(std::filesystem::path& textureFilePath) = 0;
    virtual Ref<Material> CreateMaterial(std::string& pipelineId) = 0;
};

