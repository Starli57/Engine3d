#pragma once

#include <GLFW/glfw3.h>

#include "Architecture/Rollback/Rollback.h"

#include "Rendering/Model/UboModel.h"
#include "Rendering/Vulkan/Builders/APhysicalDevice.h"
#include "Rendering/Vulkan/Builders/ALogicalDevice.h"
#include "Rendering/Vulkan/Buffers/UniformBufferVulkan.h"

namespace AVulkan
{
	class Device
	{
	public:
		void Create(VkInstance& instance, VkSurfaceKHR& surface, Rollback& renderRollback);
		void Dispose();

		VkPhysicalDevice GetPhysicalDevice();
		VkDevice GetLogicalDevice();

		VkQueue GetGraphicsQueue();
		VkQueue GetPresentationQueue();

		size_t GetModelUniformAligment();
		UboModel* GetModelUniformTransfer();

		//todo: add getters
		std::vector<UniformBufferVulkan*>* vpUniformBuffers;
		std::vector<UniformBufferVulkan*>* modelUniformBuffers;

		const uint16_t modelsPerDynamicUniform = 100;

	private:
		Rollback* deviceRollback;

		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;

		VkQueue graphicsQueue;
		VkQueue presentationQueue;

		VkDeviceSize minUniformBufferOffset;
		size_t modelUniformAligment;

		UboModel* modelUniformTransfer;
	};
}
