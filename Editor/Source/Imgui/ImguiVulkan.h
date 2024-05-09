#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "Rendering/Vulkan/VulkanGraphicsApi.h"
#include "Rendering/Vulkan/Descriptors.h"
#include "Rendering/Vulkan/SwapChain.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "Rendering/Vulkan/Builders/APhysicalDevice.h"
#include "Rendering/Vulkan/Builders/ACommandPool.h"
#include "Rendering/Vulkan/Builders/ACommandBuffer.h"

#include "SharedLib/Ref.h"
#include "SharedLib/Rollback/Rollback.h"

class ImguiVulkan : public IEngineEditor
{
public:

    ImguiVulkan(AVulkan::VulkanGraphicsApi& vulkanApi);
    ~ImguiVulkan();

    void Update() override;

private:

	AVulkan::VulkanGraphicsApi& vulkanApi;
    Ref<Rollback> rollback;

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	VkDescriptorPool descriptorPool;
	VkRenderPass renderPass;

	AVulkan::QueueFamilyIndices queueFamilies;
	uint32_t graphicsQueueFamily;

    void FrameRender(ImDrawData* draw_data);
    void FramePresent();

    void CreateRenderPass(VkDevice& logicalDevice, Ref<AVulkan::SwapChainData> swapChainData, VkRenderPass* renderPass);
};
