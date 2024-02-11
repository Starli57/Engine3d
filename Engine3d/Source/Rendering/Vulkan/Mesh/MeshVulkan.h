#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "Rendering/Model/Mesh.h"
#include "Rendering/Model/Mvp.h"
#include "Rendering/Model/Vertex.h"
#include "Rendering/Vulkan/Data/SwapChainData.h"
#include "Rendering/Vulkan/Buffers/UniformBufferVulkan.h"

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
			VkQueue& graphicsQueue, VkCommandPool& commandPool, Mesh& mesh);
		~MeshVulkan();


	private:
		VkDevice* logicalDevice;
		std::vector<Vertex>* vertices;
		std::vector<uint32_t>* indices;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

	};
}
