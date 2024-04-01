#include "Pch.h"
#include "UniformBufferVulkan.h"

namespace AVulkan
{
	UniformBufferVulkan::UniformBufferVulkan(Ref<VulkanModel> vulkanModel)
	{
		this->logicalDevice = &vulkanModel->logicalDevice;

		VkDeviceSize bufferSize = sizeof(UboViewProjectionComponent);

		VkBufferUsageFlags stagingUsageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		VkMemoryPropertyFlags stagingMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		ABuffer().Create(vulkanModel, bufferSize, stagingUsageFlags, stagingMemoryFlags, buffer, bufferMemory);

		auto mapStatus = vkMapMemory(vulkanModel->logicalDevice, bufferMemory, 0, bufferSize, 0, &bufferMapped);
		CAssert::Check(mapStatus == VK_SUCCESS, "Uniform buffer can't be created, status: " + mapStatus);
	}

	UniformBufferVulkan::~UniformBufferVulkan()
	{
		ABuffer().Dispose(*logicalDevice, buffer, bufferMemory);
	}
}