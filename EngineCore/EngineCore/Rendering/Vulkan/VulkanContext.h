#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/PipelinesCollection.h"

namespace VulkanApi
{
	class VulkanContext
	{
	public:

		GLFWwindow* window;

		VkInstance instance;
		VkSurfaceKHR windowSurface;

		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;

		VkQueue graphicsQueue;
		VkQueue presentationQueue;

		Ref<PipelinesCollection> pipelinesCollection;

		static constexpr uint16_t maxFramesInFlight = 3;
		uint64_t const frameSyncTimeout = UINT64_MAX;//todo: setup real timeout

		VkFormat imageFormat;
		VkFormat depthFormat;
		VkSampleCountFlagBits msaa;
	};
}
