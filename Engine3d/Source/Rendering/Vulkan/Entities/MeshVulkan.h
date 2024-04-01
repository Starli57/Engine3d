#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "Entities/Mesh.h"
#include "SharedLib/Components/UboViewProjectionComponent.h"
#include "Rendering/Model/Vertex.h"
#include "Rendering/Vulkan/VulkanModel.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "Rendering/Vulkan/Buffers/UniformBufferVulkan.h"
#include "SharedLib/Ref.h"

namespace AVulkan
{
	class MeshVulkan : public Mesh
	{
	public:
		VkBuffer GetVertexBuffer();
		VkBuffer GetIndexBuffer();

		MeshVulkan(Ref<VulkanModel> vulkanModel, const std::string& path);
		MeshVulkan(Ref<VulkanModel> vulkanModel, Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices);

		virtual ~MeshVulkan() override;

	private:
		VkDevice* logicalDevice;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;

	};
}
