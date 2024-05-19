#include "Pch.h"
#include "AUniformBufferVulkan.h"

namespace AVulkan
{
	Ref<BufferModel> AUniformBufferVulkan::Create(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice)
	{
		Ref<BufferModel> bufferModel = CreateRef<BufferModel>();

		VkDeviceSize bufferSize = sizeof(UboViewProjectionComponent);

		VkBufferUsageFlags stagingUsageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		VkMemoryPropertyFlags stagingMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		VkExtensions::CreateBuffer(physicalDevice, logicalDevice, bufferSize,
			stagingUsageFlags, stagingMemoryFlags, bufferModel->buffer, bufferModel->bufferMemory);

		auto mapStatus = vkMapMemory(logicalDevice, bufferModel->bufferMemory, 0, bufferSize, 0, &bufferModel->bufferMapped);
		CAssert::Check(mapStatus == VK_SUCCESS, "Uniform buffer can't be created, status: " + mapStatus);

		return bufferModel;
	}

	void AUniformBufferVulkan::Dispose(VkDevice& logicalDevice, Ref<BufferModel> bufferModel)
	{
		VkExtensions::DisposeBuffer(logicalDevice, bufferModel->buffer, bufferModel->bufferMemory);
	}
}