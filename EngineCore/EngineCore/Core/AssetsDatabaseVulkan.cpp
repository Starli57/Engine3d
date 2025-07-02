#include "EngineCore/Pch.h"
#include "AssetsDatabaseVulkan.h"

namespace EngineCore
{
    AssetsDatabaseVulkan::AssetsDatabaseVulkan(const Ref<ProjectSettings>& projectSettings) : AssetsDatabase(projectSettings)
    {
    }

    AssetsDatabaseVulkan::~AssetsDatabaseVulkan()
    {
    }
}