#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "VulkanContext.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/Models/SwapChainData.h"
#include "EngineCore/Rendering/Vulkan/Models/QueueFamilyIndices.h"

namespace VulkanApi
{
	class SwapChain
	{
	public:

		SwapChain(const Ref<VulkanContext>& context, Ref<SwapChainData> swapChainData);
		~SwapChain();

		void Recreate();
		void CreateSwapchain();
		void CreateSwapChainImageViews() const;
		void CreateDepthBuffer() const;
		void CreateMSAAColorResources() const;
		void CreateMSAADepthResources() const;
		void Dispose() const;

	private:

		Ref<SwapChainData> swapChainData;
		Ref<VulkanContext> vulkanContext;
		
		QueueFamilyIndices physicalDeviceQueueIndices;

		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& availableModes) const;
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
		VkExtent2D ChooseSwapExtent(GLFWwindow& window, const VkSurfaceCapabilitiesKHR& capabilities) const;

		void SetupSwapChainInfo(VkSwapchainCreateInfoKHR& createInfo, VkSurfaceKHR& surface, VkExtent2D& extent,
			VkPresentModeKHR& presentMode, VkSurfaceFormatKHR& surfaceFormat, VkSurfaceCapabilitiesKHR& capabilities,
			QueueFamilyIndices& physicalDeviceQueueIndices, uint32_t imageCount) const;
	};
}
