#pragma once

#include "Rendering/Model/Mesh.h"
#include "Macroses/Ref.h"

class IRenderer
{
public:
    virtual ~IRenderer() = default;

    virtual void Init() = 0;
    virtual void Render() = 0;
    virtual void FinanilizeRenderOperations() = 0;

    virtual void AddMesh(Ref<Mesh> mesh) = 0;
    virtual void CleanMeshes() = 0;

    virtual void OnFramebufferResized() = 0;
};

