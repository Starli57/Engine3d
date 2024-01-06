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
		//todo show as critical error
		spdlog::critical(e.what());
		DisposeAquiredVulkanResources();
		throw e;
	}

	//todo: delete after tests
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
	spdlog::info("Rendering GPU: {0}", deviceProperties.deviceName);

	SwapChainInterface swapChainInterface;
	SwapChainDetails details;
	swapChainInterface.GetSwapChainColorFormats(physicalDevice, windowSurface, details.formats);
	swapChainInterface.GetSwapChainPresentModes(physicalDevice, windowSurface, details.presentModes);
	for (auto colorFormat : details.formats)
	{
		spdlog::info("Available color format: {0} color space: {1}", colorFormat.format, colorFormat.colorSpace);
	}

	for (auto mode : details.presentModes)
	{
		spdlog::info("Available present mode: {0}", mode);
	}
	//end
}

VulkanRenderer::~VulkanRenderer()
{
	DisposeAquiredVulkanResources();
}

void VulkanRenderer::CreateInstance()
{
	spdlog::info("Create Instance" );
	InstanceInterface().CreateInstance(&instance);
	disposeStack.push([this]() { DisposeInstance(); });
}

void VulkanRenderer::CreateWindowSurface()
{
	spdlog::info("Create Window Surface" );
	windowSurface = WindowsSurfaceInterface().CreateSurface(instance, *window);
	disposeStack.push([this]() { DisposeSurface(); });
}

void VulkanRenderer::SelectPhysicalRenderingDevice()
{
	spdlog::info("Select physical device" );
	physicalDevice = PhysicalDeviceInterface().GetBestRenderingDevice(instance, windowSurface);
}

void VulkanRenderer::CreateLogicalDevice()
{
	spdlog::info("Create logical device" );
	logicalDevice = LogicalDeviceInterface().Create(physicalDevice, windowSurface, graphicsQueue, presentationQueue);
	disposeStack.push([this]() { DisposeLogicalDevice(); });
}

void VulkanRenderer::CreateSwapChain()
{
	spdlog::info("Create swap chain" );
	auto queueIndices = PhysicalDeviceInterface().GetQueueFamilies(physicalDevice, windowSurface);
	swapChainData = SwapChainInterface().CreateSwapChain(window, physicalDevice, logicalDevice, windowSurface, queueIndices);
	disposeStack.push([this]() { DisposeSwapChain(); });
}

void VulkanRenderer::CreateSwapChainImageViews()
{
	spdlog::info("Create swap chain image view" );
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
	spdlog::info("Dispose instance" );
	InstanceInterface().DestroyInstance(&instance);
}

void VulkanRenderer::DisposeSurface()
{
	spdlog::info("Dispose surface" );
	vkDestroySurfaceKHR(instance, windowSurface, nullptr);
}

void VulkanRenderer::DisposeLogicalDevice()
{
	spdlog::info("Dispose logical device" );
	vkDestroyDevice(logicalDevice, nullptr);
}

void VulkanRenderer::DisposeSwapChain()
{
	spdlog::info("Dispose swap chain" );
	vkDestroySwapchainKHR(logicalDevice, swapChainData.swapChain, nullptr);
}

void VulkanRenderer::DisposeSwapChainImageViews()
{
	spdlog::info("Dispose swap chain image viewes" );
	ImageViewInterface().Dispose(logicalDevice, swapChainData);
}
