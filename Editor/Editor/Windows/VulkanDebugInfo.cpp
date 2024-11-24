#include "VulkanDebugInfo.h"

VulkanDebugInfo::VulkanDebugInfo(Ref<Engine> engine, AVulkan::GraphicsApiVulkan& vulkanApi) : 
	engine(engine), vulkanApi(vulkanApi)
{
	fpsFrames = new std::vector<float>(100);
}

VulkanDebugInfo::~VulkanDebugInfo()
{
	delete fpsFrames;
}

void VulkanDebugInfo::Update()
{

	ImGui::Begin("Vulkan debug info");

	float frameFps = 1 / engine->GetDeltaTime();
	fpsFrames->at(currentFpsIndex) = frameFps;
	currentFpsIndex = (currentFpsIndex + 1) % fpsFrames->size();

	float fpsSum = 0;
	for (auto it = fpsFrames->begin(); it != fpsFrames->end(); ++it) fpsSum += *it;
	float average = fpsSum / fpsFrames->size();

	ImGui::Text("Delta time %.1ffps", average);

	ImGui::Text("Swapchain extent width=%d height=%d", vulkanApi.swapChainData->extent.width, vulkanApi.swapChainData->extent.height);

	ImGui::End();
}
