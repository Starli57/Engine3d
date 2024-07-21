#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>

#include "Extensions/Imgui_ImplVulkan.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "IEngineEditor.h"
#include "Rendering/Vulkan/GraphicsApiVulkan.h"
#include "Rendering/Vulkan/Descriptors.h"
#include "Rendering/Vulkan/SwapChain.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "Rendering/Vulkan/Utilities/PhysicalDeviceUtility.h"
#include "Rendering/Vulkan/Utilities/CommandBufferUtility.h"

#include "Rendering/Vulkan/Utilities/CommandPoolUtility.h"

#include "EngineShared/Ref.h"
#include "EngineShared/Rollback/Rollback.h"

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

	VkDescriptorPool descriptorPool;

	AVulkan::QueueFamilyIndices queueFamilies;
	uint32_t graphicsQueueFamily;
};
