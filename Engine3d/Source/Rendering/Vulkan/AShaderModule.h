#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
	class AShaderModule
	{
	public:
		VkShaderModule Create(VkDevice& logicalDevice, const std::vector<char>& shader) const;
		void Dispose(VkDevice& logicalDevice, VkShaderModule& shaderModule) const;
	};
}
