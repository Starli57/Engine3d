#include "Pch.h"
#include "VulkanRenderer.h"

namespace AVulkan
{
	VulkanRenderer::VulkanRenderer(GLFWwindow* glfwWindow, Rollback* vulkanRollback)
	{
		rollback = vulkanRollback;
		window = glfwWindow;
	}

	VulkanRenderer::~VulkanRenderer()
	{
		rollback->Dispose();
	}

	void VulkanRenderer::Initialize()
	{
		try
		{
			CreateInstance();
			CreateWindowSurface();
			SelectPhysicalRenderingDevice();
			CreateLogicalDevice();
			CreateSwapChain();
			CreateSwapChainImageViews();
			CreateRenderPass();
			CreateGraphicsPipeline();
		}
		catch (const std::exception& e)
		{
			spdlog::critical(e.what());
			rollback->Dispose();
			throw e;
		}
	}

	void VulkanRenderer::CreateInstance()
	{
		AInstance().Create(instance);
		rollback->Add([this]() { DisposeInstance(); });
	}

	void VulkanRenderer::DisposeInstance()
	{
		AInstance().Dispose(instance);
	}

	void VulkanRenderer::CreateWindowSurface()
	{
		windowSurface = AWindowsSurface().Create(instance, *window);
		rollback->Add([this]() { DisposeSurface(); });
	}

	void VulkanRenderer::DisposeSurface()
	{
		AWindowsSurface().Dispose(instance, windowSurface);
	}

	void VulkanRenderer::SelectPhysicalRenderingDevice()
	{
		APhysicalDevice pdInterface;
		physicalDevice = pdInterface.GetBestRenderingDevice(instance, windowSurface);
		pdInterface.PrintDebugInformation(physicalDevice, windowSurface);
	}

	void VulkanRenderer::CreateLogicalDevice()
	{
		logicalDevice = ALogicalDevice().Create(physicalDevice, windowSurface, graphicsQueue, presentationQueue);
		rollback->Add([this]() { DisposeLogicalDevice(); });
	}

	void VulkanRenderer::DisposeLogicalDevice()
	{
		ALogicalDevice().Dispose(logicalDevice);
	}

	void VulkanRenderer::CreateSwapChain()
	{
		auto queueIndices = APhysicalDevice().GetQueueFamilies(physicalDevice, windowSurface);
		swapChainData = ASwapChain().Create(*window, physicalDevice, logicalDevice, windowSurface, queueIndices);
		rollback->Add([this]() { DisposeSwapChain(); });
	}

	void VulkanRenderer::DisposeSwapChain()
	{
		ASwapChain().Dispose(logicalDevice, swapChainData);
	}

	void VulkanRenderer::CreateSwapChainImageViews()
	{
		AImageView().Create(logicalDevice, swapChainData);
		rollback->Add([this]() { DisposeSwapChainImageViews(); });
	}

	void VulkanRenderer::DisposeSwapChainImageViews()
	{
		AImageView().Dispose(logicalDevice, swapChainData);
	}

	void VulkanRenderer::CreateRenderPass()
	{
		renderPass = ARenderPass().Create(logicalDevice, swapChainData.imageFormat);
		rollback->Add([this]() { DisposeRenderPass(); });
	}

	void VulkanRenderer::DisposeRenderPass()
	{
		ARenderPass().Dispose(logicalDevice, renderPass);
	}

	void VulkanRenderer::CreateGraphicsPipeline()
	{
		graphicsPipelineInterface = new AGraphicsPipeline();
		graphicsPipelineInterface->Create(logicalDevice, swapChainData.extent, renderPass);

		rollback->Add([this]() { DisposeGraphicsPipeline(); });
	}

	void VulkanRenderer::DisposeGraphicsPipeline()
	{
		graphicsPipelineInterface->Dispose(logicalDevice);
		delete graphicsPipelineInterface;
	}
}