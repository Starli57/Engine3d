#include "EngineCore/Pch.h"
#include "VkShaderModuleWrapper.h"

#include "EngineCore/CustomAssert.h"
#include "spdlog/spdlog.h"

#include "EngineCore/Utilities/IOUtility.h"

namespace VulkanApi
{
	VkShaderModule CreateShaderModule(const std::string& shaderPath, const VkDevice& logicalDevice)
	{
		spdlog::info("Create shader module: {0}", shaderPath);

		const std::vector<char>& shaderCode = Engine::IOUtility().ReadFile(shaderPath);

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = shaderCode.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

		VkShaderModule shaderModule;
		auto createStatus = vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule);
		Engine::CAssert::Check(createStatus == VK_SUCCESS, "Failed to create shader module, statuc: " + createStatus);

		return shaderModule;
	}

	void DisposeShaderModule(const VkDevice& logicalDevice, const VkShaderModule& shaderModule)
	{
		vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
	}

	VkPipelineShaderStageCreateInfo SetupShaderStageInfo(const VkShaderModule& shaderModule, VkShaderStageFlagBits stage)
	{
		VkPipelineShaderStageCreateInfo stateInfo{};
		stateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		stateInfo.stage = stage;
		stateInfo.module = shaderModule;
		stateInfo.pName = "main";
		return stateInfo;
	}
}