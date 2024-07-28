#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "EngineCore/Ref.h"
#include "EngineCore/Rollback/Rollback.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainSurfaceSettings.h"
#include "EngineCore/Rendering/Vulkan/Models/QueueFamilyIndices.h"
#include "EngineCore/Rendering/Vulkan/Builders/AImage.h"
#include "EngineCore/Rendering/Vulkan/Builders/AImageView.h"
#include "EngineCore/Rendering/Vulkan/Utilities/FrameBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/FormatUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/MemoryUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/SwapchainUtility.h"

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
