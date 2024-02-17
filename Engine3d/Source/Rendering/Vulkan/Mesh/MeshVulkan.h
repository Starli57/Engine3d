#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "Rendering/Model/Mesh.h"
#include "Rendering/Model/Mvp.h"
#include "Rendering/Model/Vertex.h"
#include "Rendering/Vulkan/Data/SwapChainData.h"
#include "Rendering/Vulkan/Buffers/UniformBufferVulkan.h"
#include "Architecture/Ref.h"

namespace AVulkan
{
	class MeshVulkan : public Mesh
	{
	public:
		VkBuffer GetVertexBuffer();
		VkBuffer GetIndexBuffer();

		MeshVulkan(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, SwapChainData& swapChainData, 
			VkQueue& graphicsQueue, VkCommandPool& commandPool, Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices);
		virtual ~MeshVulkan() override;


	private:
		VkDevice* logicalDevice;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

	};
}
