#include "VulkanDebugInfo.h"

#include "EngineCore/Profiler/Profiler.h"

VulkanDebugInfo::VulkanDebugInfo(const Ref<Engine>& engine, AVulkan::GraphicsApiVulkan& vulkanApi) : 
	engine(engine), vulkanApi(vulkanApi)
{
}

VulkanDebugInfo::~VulkanDebugInfo()
{
}

void VulkanDebugInfo::Update()
{
	ImGui::Begin("Vulkan debug info");

	ShowDeltaTime("MainLoop");
	ShowDeltaTime("Rendering");
	ShowDeltaTime("RenderPassColor");
	ShowDeltaTime("RenderPassShadowMaps");
	ShowDeltaTime("Renderer Present");
	ShowDeltaTime("Renderer Submit");
	ShowDeltaTime("Systems");

	ImGui::Text("Swapchain extent width=%d height=%d", vulkanApi.swapChainData->extent.width, vulkanApi.swapChainData->extent.height);

	ImGui::End();
}

void VulkanDebugInfo::ShowDeltaTime(const std::string&& sampleName) const
{
	ImGui::Text((sampleName + " : %.2fms").c_str(), Profiler::GetInstance().GetDeltaTime(sampleName) * 1000);
}
