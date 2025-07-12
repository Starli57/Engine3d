#pragma once

#include "EngineCore/Core/ResourcesStorage.h"
#include "EngineCore/Assets/Material.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/EventHandler.h"
#include "EngineCore/Assets/Meta/MeshMeta.h"

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