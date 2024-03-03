#pragma once

#include <GLFW/glfw3.h>

namespace AVulkan
{
	class ADescriptorLayout
	{
	public:
		void Create(VkDevice& logicalDevice, VkDescriptorSetLayout& descriptorSetLayout) const;
		void Dispose(VkDevice& logicalDevice, VkDescriptorSetLayout& descriptorSetLayout) const;
	};
}