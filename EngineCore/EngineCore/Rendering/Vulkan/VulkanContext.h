#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/PipelinesCollection.h"

namespace VulkanApi
{
	const std::vector<const char*> physicalDeviceExtensions =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	struct BufferModel
	{
		VkBuffer buffer;
		VkDeviceMemory bufferMemory;
		void* bufferMapped;
	};

	struct ImageModel
	{
		VkImage image;
		VkImageView imageView;
		VkDeviceMemory imageMemory;
	};

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentationFamily;

		bool IsComplete() const
		{
			return graphicsFamily.has_value() && presentationFamily.has_value();
		}
	};
	
	struct SwapchainContext
	{
		VkSwapchainKHR swapChain;
		VkSwapchainKHR oldSwapChain;//todo: add usage to reuse resources

		VkSurfaceFormatKHR surfaceFormat;
		VkExtent2D extent;

		uint32_t imagesCount;
		std::vector<VkImage> images;
		std::vector<VkImageView> imageViews;

		ImageModel* depthBufferModel;//todo: replace to render passes
		ImageModel* msaaColorSample;//todo: replace to render passes
		ImageModel* msaaDepthSample;//todo: replace to render passes
	};

	struct SwapChainSurfaceSettings
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};
	
	struct VulkanContext
	{
		SwapchainContext* swapchainContext = nullptr;
		
		VkInstance instance = VK_NULL_HANDLE;
		VkSurfaceKHR windowSurface = VK_NULL_HANDLE;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice logicalDevice = VK_NULL_HANDLE;

		VkQueue graphicsQueue = VK_NULL_HANDLE;
		VkQueue presentationQueue = VK_NULL_HANDLE;

		VkSampler textureSampler;
		
		QueueFamilyIndices physicalDeviceQueueIndices;
		Ref<PipelinesCollection> pipelinesCollection;

		static constexpr uint16_t maxFramesInFlight = 3;
		uint64_t const frameSyncTimeout = UINT64_MAX;//todo: setup real timeout

		VkFormat imageFormat = VK_FORMAT_R8G8B8A8_SRGB;
		VkFormat depthFormat = VK_FORMAT_D32_SFLOAT;
		VkSampleCountFlagBits msaa = VK_SAMPLE_COUNT_1_BIT;
		
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> drawFences;
	};
}
