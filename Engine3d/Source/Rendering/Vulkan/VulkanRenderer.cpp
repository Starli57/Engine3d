#include "Pch.h"
#include "VulkanRenderer.h"
#include "../../Architecture/IDisposable.h"

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
		std::cout << e.what() << std::endl;
		DisposeAquiredVulkanResources();
	}

	//todo: delete after tests
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
	std::cout << "Rendering GPU: " << deviceProperties.deviceName << std::endl;

	SwapChainInterface swapChainInterface;
	SwapChainDetails details;
	swapChainInterface.GetSwapChainColorFormats(physicalDevice, windowSurface, details.formats);
	swapChainInterface.GetSwapChainPresentModes(physicalDevice, windowSurface, details.presentModes);
	for (auto colorFormat : details.formats) std::cout << "Available color format: " << colorFormat.format << " color space: " << colorFormat.colorSpace << std::endl;
	for (auto mode : details.presentModes) std::cout << "Available present mode: " << mode << std::endl;
	//end
}

VulkanRenderer::~VulkanRenderer()
{
	DisposeAquiredVulkanResources();
}

void VulkanRenderer::CreateInstance()
{
	std::cout << "Create Instance" << std::endl;
	InstanceInterface().CreateInstance(&instance);
	disposeStack.push([this]() { DisposeInstance(); });
}

void VulkanRenderer::CreateWindowSurface()
{
	std::cout << "Create Window Surface" << std::endl;
	windowSurface = WindowsSurfaceInterface().CreateSurface(instance, *window);
	disposeStack.push([this]() { DisposeSurface(); });
}

void VulkanRenderer::SelectPhysicalRenderingDevice()
{
	std::cout << "Select physical device" << std::endl;
	physicalDevice = PhysicalDeviceInterface().GetBestRenderingDevice(instance, windowSurface);
}

void VulkanRenderer::CreateLogicalDevice()
{
	std::cout << "Create loical device" << std::endl;
	logicalDevice = LogicalDeviceInterface().Create(physicalDevice, windowSurface, graphicsQueue, presentationQueue);
	disposeStack.push([this]() { DisposeLogicalDevice(); });
}

void VulkanRenderer::CreateSwapChain()
{
	std::cout << "Create swap chain" << std::endl;
	auto queueIndices = PhysicalDeviceInterface().GetQueueFamilies(physicalDevice, windowSurface);
	swapChainData = SwapChainInterface().CreateSwapChain(window, physicalDevice, logicalDevice, windowSurface, queueIndices);
	disposeStack.push([this]() { DisposeSwapChain(); });
}

void VulkanRenderer::CreateSwapChainImageViews()
{
	std::cout << "Create swap chain image view" << std::endl;
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
	std::cout << "Dispose instance" << std::endl;
	InstanceInterface().DestroyInstance(&instance);
}

void VulkanRenderer::DisposeSurface()
{
	std::cout << "Dispose surface" << std::endl;
	vkDestroySurfaceKHR(instance, windowSurface, nullptr);
}

void VulkanRenderer::DisposeLogicalDevice()
{
	std::cout << "Dispose logical device" << std::endl;
	vkDestroyDevice(logicalDevice, nullptr);
}

void VulkanRenderer::DisposeSwapChain()
{
	std::cout << "Dispose swap chain" << std::endl;
	vkDestroySwapchainKHR(logicalDevice, swapChainData.swapChain, nullptr);
}

void VulkanRenderer::DisposeSwapChainImageViews()
{
	std::cout << "Dispose swap chain image viewes" << std::endl;
	ImageViewInterface().Dispose(logicalDevice, swapChainData);
}
