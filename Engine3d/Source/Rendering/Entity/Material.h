#pragma once

#include "Texture.h"
#include "Rendering/Vulkan/AssetsDatabaseVulkan.h"
#include "Architecture/Ref.h"

class Material
{
public:
	Material(Ref<AVulkan::AssetsDatabaseVulkan> assetsDatabase, const std::string& mainTexturePath);
	~Material();

	Ref<Texture> mainTexture;
};