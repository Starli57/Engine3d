#pragma once

#include <imgui.h>
#include <imgui_impl_vulkan.h>

#include "EngineCore/EngineApi.h"
#include "EngineCore/Rendering/Vulkan/RendererVulkan.h"
#include "EngineCore/Editor/IWindow.h"

namespace UserEditor
{
	class VulkanTextureView : public Engine::IWindow
	{
	public:
		VulkanTextureView(Engine::EngineContext* engineContext);
		void Update() override;

	private:
		Engine::EngineContext* engineContext;
		std::vector<VkDescriptorSet> textureDescriptors;
		int frame = 0;
	};
}