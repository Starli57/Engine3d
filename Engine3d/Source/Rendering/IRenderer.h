#pragma once

#include "Rendering/Data/Mesh.h"

class IRenderer
{
public:
    virtual ~IRenderer() = default;

    virtual void Init() = 0;
    virtual void Render() = 0;
    virtual void FinanilizeRenderOperations() = 0;

    virtual void AddMesh(Mesh& mesh) = 0;

    virtual void OnFramebufferResized() = 0;
};

