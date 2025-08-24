#pragma once

#include <imgui.h>
#include <imgui_impl_vulkan.h>

#include "EngineCore/EngineApi.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "EngineCore/Editor/IWindow.h"

class VulkanTextureView : public Engine::IWindow
{
public:
	VulkanTextureView(const Ref<Engine::EngineApi>& engine, AVulkan::GraphicsApiVulkan& vulkanApi);
	void Update() override;

private:
	Ref<Engine::EngineApi> engine;
	AVulkan::GraphicsApiVulkan& vulkanApi;

	std::vector<VkDescriptorSet> textureDescriptors;
	int frame = 0;
};

