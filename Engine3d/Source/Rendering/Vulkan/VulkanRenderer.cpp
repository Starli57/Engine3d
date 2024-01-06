#include "Pch.h"
#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer(GLFWwindow* glfwWindow)
{
	window = glfwWindow;

	validationLayersInterface = new ValidationLayersInterface();
	physicalDevicesInterface = new PhysicalDeviceInterface();
	logicaDeviceInterface = new LogicalDeviceInterface();
	windowsSurfaceInterface = new WindowsSurfaceInterface();
	swapChainInterface = new SwapChainInterface();
	imageViewInterface = new ImageViewInterface();

	CreateInstance();
	CreateWindowSurface();
	SelectPhysicalRenderingDevice();
	CreateLogicalDevice();
	CreateSwapChain();
	CreateSwapChainImageViews();

	//todo: delete after tests
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
	std::cout << "Rendering GPU: " << deviceProperties.deviceName << std::endl;

	SwapChainDetails details;
	swapChainInterface->GetSwapChainColorFormats(physicalDevice, windowSurface, details.formats);
	swapChainInterface->GetSwapChainPresentModes(physicalDevice, windowSurface, details.presentModes);
	for (auto colorFormat : details.formats) std::cout << "Available color format: " << colorFormat.format << " color space: " << colorFormat.colorSpace << std::endl;
	for (auto mode : details.presentModes) std::cout << "Available present mode: " << mode << std::endl;
	//end
}

VulkanRenderer::~VulkanRenderer()
{
	imageViewInterface->Dispose(logicalDevice);

	delete imageViewInterface;
	delete swapChainInterface;
	delete windowsSurfaceInterface;
	delete logicaDeviceInterface;
	delete physicalDevicesInterface;
	delete validationLayersInterface;

	vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
	vkDestroyDevice(logicalDevice, nullptr);
	vkDestroySurfaceKHR(instance, windowSurface, nullptr);
	DestroyInstance();
}

void VulkanRenderer::CreateInstance()
{
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Engine3d";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Engine3d";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = GetGLFWRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	validationLayersInterface->Setup(createInfo);

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	if (result != VK_SUCCESS) std::cout << "vulkan instance can't be created: " << result << std::endl;//todo: add errors handling
}


void VulkanRenderer::SelectPhysicalRenderingDevice()
{
	physicalDevice = physicalDevicesInterface->GetBestRenderingDevice(instance, windowSurface);
}

void VulkanRenderer::CreateLogicalDevice()
{
	logicalDevice = logicaDeviceInterface->Create(physicalDevice, windowSurface, graphicsQueue, presentationQueue);
}

void VulkanRenderer::CreateWindowSurface() 
{
	windowSurface = windowsSurfaceInterface->CreateSurface(instance, *window);
}

void VulkanRenderer::CreateSwapChain()
{
	auto physicalDeviceQueueIndices = physicalDevicesInterface->GetQueueFamilies(physicalDevice, windowSurface);
	swapChain = swapChainInterface->CreateSwapChain(window, physicalDevice, logicalDevice, windowSurface, physicalDeviceQueueIndices);
}

void VulkanRenderer::CreateSwapChainImageViews()
{
	imageViewInterface->CreateImageViews(logicalDevice, swapChainInterface->swapChainImages, swapChainInterface->swapChainImageFormat);
}

std::vector<const char*> VulkanRenderer::GetGLFWRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef DEBUG
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

	std::cout << "GLFW extesions included: " << extensions.size() << std::endl;

	return extensions;
}


void VulkanRenderer::DestroyInstance()
{
	vkDestroyInstance(instance, nullptr);
}
