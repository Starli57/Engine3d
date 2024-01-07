#include "Pch.h"
#include "VulkanRenderer.h"

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
	InstanceInterface().CreateInstance(instance);
	rollback->Add([this]() { DisposeInstance(); });
}

void VulkanRenderer::CreateWindowSurface()
{
	windowSurface = WindowsSurfaceInterface().CreateSurface(instance, *window);
	rollback->Add([this]() { DisposeSurface(); });
}

void VulkanRenderer::SelectPhysicalRenderingDevice()
{
	PhysicalDeviceInterface pdInterface;
	physicalDevice = pdInterface.GetBestRenderingDevice(instance, windowSurface);
	pdInterface.PrintDebugInformation(physicalDevice, windowSurface);
}

void VulkanRenderer::CreateLogicalDevice()
{
	logicalDevice = LogicalDeviceInterface().Create(physicalDevice, windowSurface, graphicsQueue, presentationQueue);
	rollback->Add([this]() { DisposeLogicalDevice(); });
}

void VulkanRenderer::CreateSwapChain()
{
	auto queueIndices = PhysicalDeviceInterface().GetQueueFamilies(physicalDevice, windowSurface);
	swapChainData = SwapChainInterface().CreateSwapChain(*window, physicalDevice, logicalDevice, windowSurface, queueIndices);
	rollback->Add([this]() { DisposeSwapChain(); });
}

void VulkanRenderer::CreateSwapChainImageViews()
{
	ImageViewInterface().CreateImageViews(logicalDevice, swapChainData);
	rollback->Add([this]() { DisposeSwapChainImageViews(); });
}

void VulkanRenderer::DisposeInstance()
{
	InstanceInterface().DestroyInstance(instance);
}

void VulkanRenderer::DisposeSurface()
{
	WindowsSurfaceInterface().DestroySurface(instance, windowSurface);
}

void VulkanRenderer::DisposeLogicalDevice()
{
	LogicalDeviceInterface().DestroyDevice(logicalDevice);
}

void VulkanRenderer::DisposeSwapChain()
{
	SwapChainInterface().DestroySwapChain(logicalDevice, swapChainData);
}

void VulkanRenderer::DisposeSwapChainImageViews()
{
	ImageViewInterface().Dispose(logicalDevice, swapChainData);
}
