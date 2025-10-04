#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/PipelinesCollection.h"
#include "Models/QueueFamilyIndices.h"
#include "Models/SwapchainContext.h"

namespace VulkanApi
{
	class VulkanContext
	{
	public:
		SwapchainContext* swapchainContext = nullptr;
		
		GLFWwindow* window = nullptr;
		
		VkInstance instance = VK_NULL_HANDLE;
		VkSurfaceKHR windowSurface = VK_NULL_HANDLE;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice logicalDevice = VK_NULL_HANDLE;

		VkQueue graphicsQueue = VK_NULL_HANDLE;
		VkQueue presentationQueue = VK_NULL_HANDLE;

		QueueFamilyIndices physicalDeviceQueueIndices;
		Ref<PipelinesCollection> pipelinesCollection;

		static constexpr uint16_t maxFramesInFlight = 3;
		uint64_t const frameSyncTimeout = UINT64_MAX;//todo: setup real timeout

		VkFormat imageFormat = VK_FORMAT_R8G8B8A8_SRGB;
		VkFormat depthFormat = VK_FORMAT_D32_SFLOAT;
		VkSampleCountFlagBits msaa = VK_SAMPLE_COUNT_1_BIT;
	};
}
