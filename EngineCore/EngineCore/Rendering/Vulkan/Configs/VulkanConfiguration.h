#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
	struct VulkanConfiguration
	{
		static constexpr uint16_t maxFramesInFlight = 2;
		uint64_t const frameSyncTimeout = UINT64_MAX;//todo: setup real timeout

		VkFormat imageFormat;
		VkFormat depthFormat;
		VkSampleCountFlagBits msaa;
	};
}