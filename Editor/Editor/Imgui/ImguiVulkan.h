#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <imgui_internal.h>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "Editor/IEngineEditor.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#include "EngineCore/Rendering/Vulkan/Descriptors.h"
#include "EngineCore/Rendering/Vulkan/SwapChain.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Rendering/Vulkan/Utilities/PhysicalDeviceUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/CommandBufferUtility.h"

#include "EngineCore/Rendering/Vulkan/Utilities/CommandPoolUtility.h"

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rollback/Rollback.h"

class ImguiVulkan : public IEngineEditor
{
public:

    ImguiVulkan(AVulkan::GraphicsApiVulkan& vulkanApi);
    ~ImguiVulkan();

    void Update() override;

	void CreateRenderPass(VkRenderPass& renderPass);

private:

	AVulkan::GraphicsApiVulkan& vulkanApi;
    Ref<Rollback> rollback;

	VkRenderPass renderPass;
	VkDescriptorPool descriptorPool;

	AVulkan::QueueFamilyIndices queueFamilies;
	uint32_t graphicsQueueFamily;

	void DefaultEditorColors(ImGuiStyle* dst);
};
