#include "Pch.h"
#include "AShaderModule.h"

VkShaderModule AShaderModule::Create(VkDevice& logicalDevice, const std::vector<char>& shader) const
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = shader.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(shader.data());

	VkShaderModule shaderModule;
	auto createStatus = vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule);
	
	if (createStatus != VK_SUCCESS) 
	{
		throw std::runtime_error("Failed to create shader module!");
	}

	return shaderModule;
}

void AShaderModule::Dispose(VkDevice& logicalDevice, VkShaderModule& shaderModule) const
{
	vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
}
