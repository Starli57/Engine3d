#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <imgui_internal.h>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "EngineCore/Editor/IEngineEditor.h"
#include "EngineCore/Rendering/Vulkan/RendererVulkan.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/Rollback/Rollback.h"

namespace Editor
{
	class ImguiVulkan : public Engine::IEngineEditor
	{
	public:
		ImguiVulkan(const Ref<ProjectSettings>& projectSettings, Engine::RendererVulkan& renderer);
		~ImguiVulkan();

		void Update() override;

		void CreateRenderPass(VkRenderPass& renderPass) const;

	private:
		Ref<ProjectSettings> projectSettings;
		Engine::RendererVulkan& renderer;
		Ref<Engine::Rollback> rollback;

		VkRenderPass renderPass;
		uint32_t graphicsQueueFamily;

		void DefaultEditorColors(ImGuiStyle* dst) const;
	};
}