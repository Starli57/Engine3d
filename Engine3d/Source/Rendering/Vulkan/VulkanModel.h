#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "SharedLib/Ref.h"
#include "Rendering/Vulkan/GraphicsPipeline.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"

namespace AVulkan
{
	struct VulkanModel
	{
		GLFWwindow* window;

		VkInstance instance;
		VkSurfaceKHR windowSurface;

		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;

		VkQueue graphicsQueue;
		VkQueue presentationQueue;

		VkRenderPass renderPass;

		VkCommandPool commandPool;

		VkDescriptorSetLayout descriptorSetLayout;
		VkDescriptorPool descriptorPool;

		Ref<SwapChainData> swapChainData;
		Ref<GraphicsPipeline> graphicsPipeline;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> drawFences;

		uint16_t frame = 0;
		uint16_t const maxFramesInFlight = 2;
		uint64_t const frameSyncTimeout = UINT64_MAX;//todo: setup real timeout

		VkSampler textureSampler;

		bool needResizeWindow = false;
	};
}