#pragma once

#include <imgui.h>
#include <imgui_impl_vulkan.h>

#include "EngineCore/EngineApi.h"
#include "EngineCore/Rendering/Vulkan/RendererVulkan.h"
#include "EngineCore/Editor/IWindow.h"

class VulkanTextureView : public Engine::IWindow
{
public:
	VulkanTextureView(const Ref<Engine::EngineApi>& engine, Engine::RendererVulkan& renderer);
	void Update() override;

private:
	Ref<Engine::EngineApi> engine;
	Engine::RendererVulkan& renderer;

	std::vector<VkDescriptorSet> textureDescriptors;
	int frame = 0;
};

