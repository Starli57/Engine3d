#pragma once

namespace Engine
{
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;

        virtual void Init() = 0;
        virtual void Render() = 0;
        virtual void FinalizeRenderOperations() = 0;
    };
}