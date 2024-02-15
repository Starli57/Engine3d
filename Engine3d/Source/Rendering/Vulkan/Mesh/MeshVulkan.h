#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "Rendering/Model/Mesh.h"
#include "Rendering/Model/UboViewProjection.h"
#include "Rendering/Model/Vertex.h"
#include "Rendering/Vulkan/Model/SwapChainData.h"
#include "Rendering/Vulkan/Buffers/UniformBufferVulkan.h"
#include "Macroses/Ref.h"

namespace AVulkan
{
	class MeshVulkan
	{
	public:
		uint32_t GetVertexCount();
		VkBuffer GetVertexBuffer();

		uint32_t GetIndicesCount();
		VkBuffer GetIndexBuffer();

		MeshVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, SwapChainData& swapChainData, 
			VkQueue& graphicsQueue, VkCommandPool& commandPool, Ref<Mesh> mesh);
		~MeshVulkan();


	private:
		VkDevice* logicalDevice;
		Ref<std::vector<Vertex>> vertices;
		Ref<std::vector<uint32_t>> indices;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

	};
}
