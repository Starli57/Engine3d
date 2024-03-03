#include "Pch.h"
#include "AShaderModule.h"

#include "SharedLib/IOUtility.h"

namespace AVulkan
{
	VkShaderModule AShaderModule::CreateModule(const std::string& fileName, VkDevice& logicalDevice) const
	{
		spdlog::info("Create shader module: {0}", fileName);

		auto shadersFolderPath = "../Engine3d/Source/Rendering/Shaders/";
		auto shaderPath = shadersFolderPath + fileName;

		const std::vector<char>& shaderCode = IOUtility().ReadFile(shaderPath);

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = shaderCode.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

		VkShaderModule shaderModule;
		auto createStatus = vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule);
		CAssert::Check(createStatus == VK_SUCCESS, "Failed to create shader module, statuc: " + createStatus);

		return shaderModule;
	}

	void AShaderModule::DisposeModule(VkDevice& logicalDevice, VkShaderModule& shaderModule) const
	{
		vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
	}

	VkPipelineShaderStageCreateInfo AShaderModule::SetupStageInfo(VkShaderModule& shaderModule, VkShaderStageFlagBits stage) const
	{
		VkPipelineShaderStageCreateInfo stateInfo{};
		stateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		stateInfo.stage = stage;
		stateInfo.module = shaderModule;
		stateInfo.pName = "main";
		return stateInfo;
	}
}