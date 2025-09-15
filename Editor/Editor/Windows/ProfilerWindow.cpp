#include "ProfilerWindow.h"

#include "EngineCore/Components/IdComponent.h"
#include "EngineCore/Components/NameComponent.h"
#include "EngineCore/Profiler/Profiler.h"

ProfilerWindow::ProfilerWindow(const Ref<Engine::EngineApi>& engine, VulkanApi::GraphicsApiVulkan& vulkanApi) : 
	engine(engine), vulkanApi(vulkanApi)
{
}

void ProfilerWindow::Update()
{
	ImGui::Begin("Vulkan debug info");

	auto profiler = Engine::Profiler::GetInstance();
	auto samples = profiler.samples;
	for (size_t i = 0; i < samples.size(); i++) ShowRecursive(samples[i]);

	ImGui::Text("Triangles: %d", profiler.GetTrianglesCount());
	ImGui::Text("Draw Calls: %d", profiler.GetDrawCalls());
	ImGui::Text("Swapchain extent width=%d height=%d", vulkanApi.swapChainData->extent.width, vulkanApi.swapChainData->extent.height);

	double screenPositionX;
	double screenPositionY;
	engine->GetInput()->GetCursorPosition(screenPositionX, screenPositionY);
	ImGui::Text("Cursor position %.0f %.0f", screenPositionX, screenPositionY);
	ImGui::Text("Is mouse 0 pressed %s", engine->GetInput()->IsMousePressedStay(0) ? "true" : "false");
	
	ImGui::End();
}

void ProfilerWindow::ShowRecursive(Engine::ProfilerSample& sample) const
{
	if (ImGui::TreeNode(sample.sampleName.c_str(), (sample.sampleName + " : %.3fms").c_str(), Engine::Profiler::GetInstance().GetDeltaTime(sample) * 1000))
	{
		for (auto& childrenSample : sample.childrenSamples)
		{
			ShowRecursive(childrenSample);
		}
		ImGui::TreePop();
		ImGui::Spacing();
	}
}
