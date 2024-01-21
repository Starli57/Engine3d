#pragma once

#include <GLFW/glfw3.h>
#include <string>

namespace AVulkan
{
	class AShaderModule
	{
	public:
		VkShaderModule CreateModule(const std::string& fileName, VkDevice& logicalDevice) const;
		void DisposeModule(VkDevice& logicalDevice, VkShaderModule& shaderModule) const;

		VkPipelineShaderStageCreateInfo SetupStageInfo(VkShaderModule& shaderModule, VkShaderStageFlagBits stage) const;
	};
}
