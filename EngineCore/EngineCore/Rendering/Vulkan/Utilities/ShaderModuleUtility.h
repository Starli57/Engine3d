#pragma once

#include <GLFW/glfw3.h>
#include <string>

namespace VkUtils
{
	VkShaderModule CreateShaderModule(const std::string& shaderPath, VkDevice& logicalDevice);
	void DisposeShaderModule(VkDevice& logicalDevice, VkShaderModule& shaderModule);

	VkPipelineShaderStageCreateInfo SetupShaderStageInfo(VkShaderModule& shaderModule, VkShaderStageFlagBits stage);;
}
