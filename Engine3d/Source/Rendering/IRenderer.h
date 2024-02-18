#pragma once

#include "Rendering/Model/Mesh.h"
#include "Architecture/Ref.h"

class IRenderer
{
public:
    virtual ~IRenderer() = default;

    virtual void Init() = 0;
    virtual void Render() = 0;
    virtual void FinanilizeRenderOperations() = 0;

    virtual Ref<Mesh> CreateMesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices) = 0;

    virtual void OnFramebufferResized() = 0;
};

