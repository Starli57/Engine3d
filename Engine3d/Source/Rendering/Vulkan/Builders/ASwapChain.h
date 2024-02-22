#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "Rendering/Vulkan/Models/SwapChainSurfaceSettings.h"
#include "Rendering/Vulkan/Models/QueueFamilyIndices.h"

namespace AVulkan
{
	class ASwapChain
	{
	public:
		void Create(GLFWwindow& window, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice,
			VkSurfaceKHR& surface, QueueFamilyIndices& physicalDeviceQueueIndices, SwapChainData& swapChainData) const;
		void Dispose(VkDevice& logicalDevice, SwapChainData& swapChainData) const;

		SwapChainSurfaceSettings GetSwapChainDetails(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) const;
		void GetSwapChainColorFormats(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, std::vector<VkSurfaceFormatKHR>& formats) const;
		void GetSwapChainPresentModes(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, std::vector<VkPresentModeKHR>& presentModes) const;

		bool DoSupportSwapChain(SwapChainSurfaceSettings& surfaceSettings) const;
		bool DoSupportSwapChain(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) const;

	private:
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& availableModes) const;
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
		VkExtent2D ChooseSwapExtent(GLFWwindow& window, const VkSurfaceCapabilitiesKHR& capabilities) const;

		void SetupSwapChainInfo(VkSwapchainCreateInfoKHR& createInfo, VkSurfaceKHR& surface, VkExtent2D& extent,
			VkPresentModeKHR& presentMode, VkSurfaceFormatKHR& surfaceFormat, VkSurfaceCapabilitiesKHR& capabilities,
			QueueFamilyIndices& physicalDeviceQueueIndices, uint32_t imageCount) const;
	};
}
