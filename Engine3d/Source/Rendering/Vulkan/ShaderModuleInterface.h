#pragma once

#include <GLFW/glfw3.h>

class ShaderModuleInterface
{
public:
	VkShaderModule Create(VkDevice& logicalDevice, const std::vector<char>& shader) const;
	void Destroy(VkDevice& logicalDevice, VkShaderModule& shaderModule) const;
};

