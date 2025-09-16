#include "EngineCore/Pch.h"
#include "EngineCore/Rendering/Vulkan/ApiWrappers/VkBufferWrapper.h"
#include "VertexBufferUtility.h"
#include "spdlog/spdlog.h"

namespace VulkanApi
{
	void CreateVertexBuffer(VulkanContext* vulkanContext, const std::vector<Engine::Vertex>& vertices,
		VkBuffer& vertexBuffer, VkDeviceMemory& bufferMemory, VkCommandPool& commandPool)
	{
		uint64_t bufferSize = sizeof(Engine::Vertex) * vertices.size();
		VkBufferUsageFlags distUsageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		CreateDeviceLocalBuffer(vulkanContext, bufferSize, vertices.data(), distUsageFlags, vertexBuffer, bufferMemory, commandPool);
	}

	void DisposeVertexBuffer(const VkDevice& logicalDevice, const VkBuffer& buffer, const VkDeviceMemory& bufferMemory)
	{
		if (buffer == VK_NULL_HANDLE) return;
		DisposeBuffer(logicalDevice, buffer, bufferMemory);
	}

	VkVertexInputBindingDescription GetVertexInputBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Engine::Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	std::array<VkVertexInputAttributeDescription, 4> GetVertexInputAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Engine::Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Engine::Vertex, normal);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Engine::Vertex, uv);

		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(Engine::Vertex, color);

		return attributeDescriptions;
	}
}