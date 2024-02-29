#pragma once

#include "AssetsDatabase.h"
#include "Rendering/Entity/Mesh.h"
#include "Rendering/Entity/Texture.h"
#include "Architecture/Ref.h"

class IGraphicsApi
{
public:
    virtual ~IGraphicsApi() = default;

    virtual void Init() = 0;
    virtual void Render() = 0;
    virtual void FinanilizeRenderOperations() = 0;

    virtual Ref<Mesh> CreateMesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices) = 0;
    virtual Ref<Texture> CreateTexture(const std::string& filePath) = 0;

    virtual void OnFramebufferResized() = 0;
};

