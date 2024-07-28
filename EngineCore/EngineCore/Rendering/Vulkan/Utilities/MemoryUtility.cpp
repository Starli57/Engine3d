#include "EngineCore/Pch.h"
#include "MemoryUtility.h"

namespace VkUtils
{
	uint32_t FindMemoryType(VkPhysicalDevice& physicalDevice, uint32_t& typeFilter, VkMemoryPropertyFlags& properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("Failed to find suitable memory type!");
	}

	void FreeDeviceMemory(VkDevice& logicalDevice, VkDeviceMemory& memory)
	{
		vkFreeMemory(logicalDevice, memory, nullptr);
	}
}