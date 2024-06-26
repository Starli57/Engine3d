#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "EngineShared/Ref.h"
#include "EngineShared/Rollback/Rollback.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "Rendering/Vulkan/Models/SwapChainSurfaceSettings.h"
#include "Rendering/Vulkan/Models/QueueFamilyIndices.h"
#include "Rendering/Vulkan/Builders/AImage.h"
#include "Rendering/Vulkan/Builders/AImageView.h"
#include "Rendering/Vulkan/Utilities/FrameBufferUtility.h"
#include "Rendering/Vulkan/Utilities/FormatUtility.h"
#include "Rendering/Vulkan/Utilities/MemoryUtility.h"
#include "Rendering/Vulkan/Utilities/SwapchainUtility.h"

namespace AVulkan
{
	class SwapChain
	{
	public:

		Ref<ImageModel> depthBufferModel;

		SwapChain(Ref<Rollback> rollback, GLFWwindow& window, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkSurfaceKHR& surface,
			VkQueue& graphicsQueue, Ref<SwapChainData> swapChainData);
		~SwapChain();

		void Recreate();
		void CreateSwapchain();
		void CreateSwapChainImageViews();
		void CreateDepthBuffer(VkCommandPool& commandPool);
		void CreateFrameBuffers(VkRenderPass& renderPass);
		void Dispose();

	private:

		Ref<Rollback> rollback;
		Ref<SwapChainData> swapChainData;

		GLFWwindow& window;
		VkPhysicalDevice& physicalDevice;
		VkDevice& logicalDevice;
		VkSurfaceKHR& surface;
		VkQueue& graphicsQueue;

		QueueFamilyIndices physicalDeviceQueueIndices;
		VkCommandPool commandPool;
		VkRenderPass renderPass;

		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& availableModes) const;
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
		VkExtent2D ChooseSwapExtent(GLFWwindow& window, const VkSurfaceCapabilitiesKHR& capabilities) const;

		void SetupSwapChainInfo(VkSwapchainCreateInfoKHR& createInfo, VkSurfaceKHR& surface, VkExtent2D& extent,
			VkPresentModeKHR& presentMode, VkSurfaceFormatKHR& surfaceFormat, VkSurfaceCapabilitiesKHR& capabilities,
			QueueFamilyIndices& physicalDeviceQueueIndices, uint32_t imageCount) const;
	};
}
