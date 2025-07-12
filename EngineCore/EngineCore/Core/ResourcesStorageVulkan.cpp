#include "EngineCore/Pch.h"
#include "ResourcesStorageVulkan.h"

namespace EngineCore
{
    ResourcesStorageVulkan::ResourcesStorageVulkan(const Ref<ProjectSettings>& projectSettings) : ResourcesStorage(projectSettings)
    {
    }

    ResourcesStorageVulkan::~ResourcesStorageVulkan()
    {
    }
}