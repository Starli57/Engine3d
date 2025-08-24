#pragma once

namespace EngineCore
{
    class IGraphicsApi
    {
    public:
        virtual ~IGraphicsApi() = default;

        virtual void Init() = 0;
        virtual void Render() = 0;
        virtual void FinalizeRenderOperations() = 0;
    };
}