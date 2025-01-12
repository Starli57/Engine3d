#include "VulkanDebugInfo.h"

VulkanDebugInfo::VulkanDebugInfo(Ref<Engine> engine, AVulkan::GraphicsApiVulkan& vulkanApi) : 
	engine(engine), vulkanApi(vulkanApi)
{
	frameTimes = new std::vector<float>(100);
}

VulkanDebugInfo::~VulkanDebugInfo()
{
	delete frameTimes;
}

void VulkanDebugInfo::Update()
{

	ImGui::Begin("Vulkan debug info");

	frameTimes->at(currentIndex) = engine->GetDeltaTime();
	currentIndex = (currentIndex + 1) % frameTimes->size();

	float sum = 0;
	for (auto it = frameTimes->begin(); it != frameTimes->end(); ++it) sum += *it;
	float average = sum / frameTimes->size();

	ImGui::Text("Delta time %.2fms", average * 1000);

	ImGui::Text("Swapchain extent width=%d height=%d", vulkanApi.swapChainData->extent.width, vulkanApi.swapChainData->extent.height);

	ImGui::End();
}
