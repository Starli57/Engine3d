#include "Pch.h"
#include "Device.h"

namespace AVulkan
{
	void Device::Create(VkInstance& instance, VkSurfaceKHR& surface, Rollback& renderRollback)
	{
		deviceRollback = new Rollback(renderRollback);

		APhysicalDevice pdInterface;
		physicalDevice = pdInterface.GetBestRenderingDevice(instance, surface);

		VkPhysicalDeviceLimits deviceLimits;
		pdInterface.SetupDeviceLimits(physicalDevice, deviceLimits);
		minUniformBufferOffset = deviceLimits.minUniformBufferOffsetAlignment;

		modelUniformAligment = (sizeof(UboModel) + minUniformBufferOffset - 1) & ~(minUniformBufferOffset - 1);
		modelUniformTransfer = (UboModel*)_aligned_malloc(modelUniformAligment * modelsPerDynamicUniform, modelUniformAligment);

		pdInterface.PrintDebugInformation(physicalDevice, surface);
		
		logicalDevice = ALogicalDevice().Create(physicalDevice, surface, graphicsQueue, presentationQueue);
		deviceRollback->Add([this]() 
		{
			ALogicalDevice().Dispose(logicalDevice); 
			_aligned_free(modelUniformTransfer);
		});

	}

	void Device::Dispose()
	{
		deviceRollback->Dispose();
	}

	VkPhysicalDevice Device::GetPhysicalDevice() { return physicalDevice; }
	VkDevice Device::GetLogicalDevice() { return logicalDevice; }

	VkQueue Device::GetGraphicsQueue() { return graphicsQueue; }
	VkQueue Device::GetPresentationQueue() { return presentationQueue; }

	size_t Device::GetModelUniformAligment() { return modelUniformAligment; }
	UboModel* Device::GetModelUniformTransfer() { return modelUniformTransfer; }
}