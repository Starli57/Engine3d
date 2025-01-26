#pragma once

#include <GLFW/glfw3.h>
#include <string>

namespace VkUtils
{
	VkShaderModule CreateShaderModule(const std::string& shaderPath, const VkDevice& logicalDevice);
	void DisposeShaderModule(const VkDevice& logicalDevice, const VkShaderModule& shaderModule);

	VkPipelineShaderStageCreateInfo SetupShaderStageInfo(const VkShaderModule& shaderModule, VkShaderStageFlagBits stage);;
}
