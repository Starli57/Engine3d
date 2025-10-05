#pragma once
#include <GLFW/glfw3.h>

#include "Core/AssetsLoaderVulkan.h"
#include "Core/ProjectSettings.h"
#include "Core/Ref.h"
#include "Core/SystemsState.h"
#include "Rendering/Vulkan/RendererVulkan.h"
#include "Serialization/EntitySerializer.h"

namespace Engine
{
    class EngineContext
    {
    public:
        EngineContext(const Ref<ProjectSettings>& settings) : projectSettings(settings) {}
        
        GLFWwindow* window = nullptr;
        
#if GLFW_INCLUDE_VULKAN
        RendererVulkan* renderer = nullptr;
        Ref<ResourcesStorageVulkan> resourcesStorage;
        Ref<AssetsLoaderVulkan> resourcesManager;
#else
        IRenderer* renderer;
        Ref<ResourcesStorage> resourcesStorage;
        Ref<AssetsLoader> resourcesManager;
#endif
        
        const Ref<ProjectSettings>& projectSettings;
        
        Ref<Ecs> ecs;
        Ref<SystemsState> systemsState;
        Ref<EntitySerializer> entitySerializer;
        Ref<InputManager> input;
    };
}
