#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>

#include "Architecture/Ref.h"
#include "Rendering/Vulkan/Entities/TextureVulkan.h"

namespace AVulkan
{
	class AssetsDatabaseVulkan
	{
	public:
		AssetsDatabaseVulkan();
		~AssetsDatabaseVulkan();

		bool HasTexture(std::string path);
		Ref<TextureVulkan> GetTexture(std::string path);
		void AddTexture(Ref<TextureVulkan> texture);
		void RemoveTexture(Ref<TextureVulkan> texture);

	private:
		std::unordered_map<std::string, Ref<TextureVulkan>> textures;
	};
}