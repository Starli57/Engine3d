#include "VulkanDebugInfo.h"

VulkanDebugInfo::VulkanDebugInfo(Ref<Engine> engine, AVulkan::GraphicsApiVulkan& vulkanApi) : 
	engine(engine), vulkanApi(vulkanApi)
{
}

void VulkanDebugInfo::Update()
{

	ImGui::Begin("Vulkan debug info");

	ImGui::Text("Delta time %.3fms", engine->GetDeltaTime());

	ImGui::Text("Swapchain extent width=%d height=%d", vulkanApi.swapChainData->extent.width, vulkanApi.swapChainData->extent.height);

	ImGui::End();
}
