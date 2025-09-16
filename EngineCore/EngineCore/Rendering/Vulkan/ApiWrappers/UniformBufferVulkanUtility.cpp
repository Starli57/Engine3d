#include "EngineCore/Pch.h"
#include "UniformBufferVulkanUtility.h"

#include "VkBufferWrapper.h"
#include "EngineCore/CustomAssert.h"

namespace VulkanApi
{
	Ref<BufferModel> CreateUniformBuffer(const VulkanContext* vulkanContext, const VkDeviceSize bufferSize)
	{
		Ref<BufferModel> bufferModel = CreateRef<BufferModel>();

		VkBufferUsageFlags stagingUsageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		constexpr VkMemoryPropertyFlags stagingMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		CreateBuffer(vulkanContext, bufferSize, stagingUsageFlags, stagingMemoryFlags, bufferModel->buffer, bufferModel->bufferMemory);

		const auto mapStatus = vkMapMemory(vulkanContext->logicalDevice, bufferModel->bufferMemory, 0, bufferSize, 0, &bufferModel->bufferMapped);
		Engine::CAssert::Check(mapStatus == VK_SUCCESS, "Uniform buffer can't be created, status: " + mapStatus);

		return bufferModel;
	}
}
