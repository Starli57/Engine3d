#pragma once

#include <imgui.h>
#include <imgui_impl_vulkan.h>

#include "EngineCore/Engine.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "EngineCore/Editor/IWindow.h"

class VulkanTextureView : public IWindow
{
public:
	VulkanTextureView(Ref<Engine> engine, AVulkan::GraphicsApiVulkan& vulkanApi);
	void Update() override;

private:
	Ref<Engine> engine;
	AVulkan::GraphicsApiVulkan& vulkanApi;

	VkDescriptorSet textureDescriptor;
};

