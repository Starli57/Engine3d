#pragma once

class IRenderer
{
public:
    virtual ~IRenderer() = default;

    virtual void Initialize() = 0;
    virtual void Render() = 0;
    virtual void FinanilizeRenderOperations() = 0;

    virtual void OnFramebufferResized() = 0;
};

