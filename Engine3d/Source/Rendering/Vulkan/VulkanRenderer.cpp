#include "Pch.h"
#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer(GLFWwindow* glfwWindow)
{
	window = glfwWindow;

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
		DisposeAquiredVulkanResources();
		throw e;
	}
}

VulkanRenderer::~VulkanRenderer()
{
	DisposeAquiredVulkanResources();
}

void VulkanRenderer::CreateInstance()
{
	InstanceInterface().CreateInstance(instance);
	disposeStack.push([this]() { DisposeInstance(); });
}

void VulkanRenderer::CreateWindowSurface()
{
	windowSurface = WindowsSurfaceInterface().CreateSurface(instance, *window);
	disposeStack.push([this]() { DisposeSurface(); });
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
	disposeStack.push([this]() { DisposeLogicalDevice(); });
}

void VulkanRenderer::CreateSwapChain()
{
	auto queueIndices = PhysicalDeviceInterface().GetQueueFamilies(physicalDevice, windowSurface);
	swapChainData = SwapChainInterface().CreateSwapChain(*window, physicalDevice, logicalDevice, windowSurface, queueIndices);
	disposeStack.push([this]() { DisposeSwapChain(); });
}

void VulkanRenderer::CreateSwapChainImageViews()
{
	ImageViewInterface().CreateImageViews(logicalDevice, swapChainData);
	disposeStack.push([this]() { DisposeSwapChainImageViews(); });
}

void VulkanRenderer::DisposeAquiredVulkanResources()
{
	while (!disposeStack.empty())
	{
		auto disposable = disposeStack.top();
		disposable();
		disposeStack.pop();
	}
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
