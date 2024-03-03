
#include "Pch.h"
#include "UniformBufferVulkan.h"

namespace AVulkan
{
	UniformBufferVulkan::UniformBufferVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice)
	{
		this->logicalDevice = &logicalDevice;

		VkDeviceSize bufferSize = sizeof(UboViewProjection);

		VkBufferUsageFlags stagingUsageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		VkMemoryPropertyFlags stagingMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		ABuffer().Create(physicalDevice, logicalDevice, bufferSize, 
			stagingUsageFlags, stagingMemoryFlags, buffer, bufferMemory);

		auto mapStatus = vkMapMemory(logicalDevice, bufferMemory, 0, bufferSize, 0, &bufferMapped);
		CAssert::Check(mapStatus == VK_SUCCESS, "Uniform buffer can't be created, status: " + mapStatus);
	}

	UniformBufferVulkan::~UniformBufferVulkan()
	{
		ABuffer().Dispose(*logicalDevice, buffer, bufferMemory);
	}
}