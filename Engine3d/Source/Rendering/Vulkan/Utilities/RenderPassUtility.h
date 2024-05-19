#pragma once

#include <GLFW/glfw3.h>

namespace VkUtils
{
	VkRenderPass CreateRenderPass(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkFormat& scImageFormat);
	void DisposeRenderPass(VkDevice& logicalDevice, VkRenderPass& renderPass);
}