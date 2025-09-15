#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <imgui_internal.h>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "Editor/IEngineEditor.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "EngineCore/Rendering/Vulkan/SwapChain.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Rendering/Vulkan/Utilities/PhysicalDeviceUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/CommandBufferUtility.h"

#include "EngineCore/Rendering/Vulkan/Utilities/CommandPoolUtility.h"

#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/Rollback/Rollback.h"

class ImguiVulkan : public IEngineEditor
{
public:
    ImguiVulkan(const Ref<ProjectSettings>& projectSettings, VulkanApi::GraphicsApiVulkan& vulkanApi);
    ~ImguiVulkan();

    void Update() override;

	void CreateRenderPass(VkRenderPass& renderPass) const;

private:
	Ref<ProjectSettings> projectSettings;
	VulkanApi::GraphicsApiVulkan& vulkanApi;
    Ref<Engine::Rollback> rollback;

	VkRenderPass renderPass;

	VulkanApi::QueueFamilyIndices queueFamilies;
	uint32_t graphicsQueueFamily;

	void DefaultEditorColors(ImGuiStyle* dst) const;
};
