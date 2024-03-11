#pragma once

#include "AssetsDatabase.h"
#include "Entities/Mesh.h"
#include "Entities/Texture.h"
#include "SharedLib/Ref.h"
#include "SharedLib/EventHandler.h"

class IGraphicsApi
{
public:
    virtual ~IGraphicsApi() = default;

    virtual void Init() = 0;
    virtual void Render() = 0;
    virtual void FinanilizeRenderOperations() = 0;

    virtual Ref<Mesh> CreateMesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices) = 0;
    virtual Ref<Texture> CreateTexture(TextureId textureId) = 0;

    virtual void OnFramebufferResized() = 0;

    EventHandler<float> OnFrameBufferAspectRatioChanged;
};

