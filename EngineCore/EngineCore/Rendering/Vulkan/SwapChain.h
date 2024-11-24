#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainSurfaceSettings.h"
#include "EngineCore/Rendering/Vulkan/Models/QueueFamilyIndices.h"
#include "EngineCore/Rendering/Vulkan/Utilities/ImageUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/FrameBufferUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/FormatUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/MemoryUtility.h"
#include "EngineCore/Rendering/Vulkan/Utilities/SwapchainUtility.h"
#include "EngineCore/Rendering/Vulkan/Configs/VulkanConfiguration.h"
#include "EngineCore/Rendering/Vulkan/RenderPasses/RenderPassColor.h"
#include "EngineCore/Rendering/Vulkan/RenderPasses/RenderPassShadowMaps.h"

namespace AVulkan
{
	class SwapChain
	{
	public:

		SwapChain(
			GLFWwindow& window, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkSurfaceKHR& surface,
			VkQueue& graphicsQueue, Ref<SwapChainData> swapChainData, Ref<VulkanConfiguration> rendererConfig);
		~SwapChain();

		void Recreate();
		void CreateSwapchain();
		void CreateSwapChainImageViews();
		void CreateDepthBuffer();
		void CreateMSAAColorResources();
		void CreateMSAADepthResources();
		void Dispose();

	private:

		Ref<SwapChainData> swapChainData;
		Ref<VulkanConfiguration> rendererConfig;
		
		GLFWwindow& window;
		VkPhysicalDevice& physicalDevice;
		VkDevice& logicalDevice;
		VkSurfaceKHR& surface;
		VkQueue& graphicsQueue;
		
		QueueFamilyIndices physicalDeviceQueueIndices;

		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& availableModes) const;
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
		VkExtent2D ChooseSwapExtent(GLFWwindow& window, const VkSurfaceCapabilitiesKHR& capabilities) const;

		void SetupSwapChainInfo(VkSwapchainCreateInfoKHR& createInfo, VkSurfaceKHR& surface, VkExtent2D& extent,
			VkPresentModeKHR& presentMode, VkSurfaceFormatKHR& surfaceFormat, VkSurfaceCapabilitiesKHR& capabilities,
			QueueFamilyIndices& physicalDeviceQueueIndices, uint32_t imageCount) const;
	};
}
