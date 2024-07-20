#include "VulkanDebugInfo.h"

VulkanDebugInfo::VulkanDebugInfo(AVulkan::VulkanGraphicsApi& vulkanApi) : vulkanApi(vulkanApi)
{
}

void VulkanDebugInfo::Update()
{

	ImGui::Begin("Vulkan debug info");

	char label[128];
	sprintf_s(label, "Swapchain extent width=%d height=%d", vulkanApi.swapChainData->extent.width, vulkanApi.swapChainData->extent.height);
	ImGui::LabelText("", "%s", label);

	ImGui::End();
}
