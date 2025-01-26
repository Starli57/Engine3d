#include "EngineCore/Pch.h"
#include "UniformBufferVulkanUtility.h"

namespace VkUtils
{
	Ref<AVulkan::BufferModel> CreateUniformBuffer(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, const VkDeviceSize bufferSize)
	{
		Ref<AVulkan::BufferModel> bufferModel = CreateRef<AVulkan::BufferModel>();

		VkBufferUsageFlags stagingUsageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		constexpr VkMemoryPropertyFlags stagingMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		CreateBuffer(physicalDevice, logicalDevice, bufferSize,
			stagingUsageFlags, stagingMemoryFlags, bufferModel->buffer, bufferModel->bufferMemory);

		const auto mapStatus = vkMapMemory(logicalDevice, bufferModel->bufferMemory, 0, bufferSize, 0, &bufferModel->bufferMapped);
		CAssert::Check(mapStatus == VK_SUCCESS, "Uniform buffer can't be created, status: " + mapStatus);

		return bufferModel;
	}
}