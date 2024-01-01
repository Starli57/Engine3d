#include "Pch.h"
#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer(GLFWwindow* glfwWindow)
{
	window = glfwWindow;

	validationLayers = new ValidationLayers();
	physycalDeviceSelector = new PhysicalDeviceSelector();
	logicaDeviceCreator = new LogicalDeviceCreator();
	windowsSurfaceCreator = new WindowsSurface();

	CreateInstance();
	CreateWindowSurface();
	SelectPhysicalRenderingDevice();
	CreateLogicalDevice();

	//todo: delete after tests
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
	std::cout << "Rendering GPU: " << deviceProperties.deviceName << std::endl;
}

VulkanRenderer::~VulkanRenderer()
{
	delete windowsSurfaceCreator;
	delete logicaDeviceCreator;
	delete physycalDeviceSelector;
	delete validationLayers;

	vkDestroyDevice(logicalDevice, nullptr);
	vkDestroySurfaceKHR(instance, windowSurface, nullptr);
	DestroyInstance();
}

void VulkanRenderer::CreateInstance()
{
	VkApplicationInfo appInfo{};
	SetupAppInfo(appInfo);

	VkInstanceCreateInfo createInfo{};
	SetupInstanceCreateInfo(createInfo, appInfo);
	validationLayers->Setup(createInfo);

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	if (result != VK_SUCCESS) std::cout << "vulkan instance can't be created: " << result << std::endl;//todo: add errors handling
}

void VulkanRenderer::SetupAppInfo(VkApplicationInfo& appInfo)
{
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Engine3d";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Engine3d";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;
}

void VulkanRenderer::SetupInstanceCreateInfo(VkInstanceCreateInfo& createInfo, VkApplicationInfo& appInfo)
{
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;


	auto extensions = GetGLFWRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	createInfo.enabledLayerCount = 0;
}

void VulkanRenderer::SelectPhysicalRenderingDevice()
{
	physicalDevice = physycalDeviceSelector->GetBestRenderingDevice(instance, windowSurface);
}

void VulkanRenderer::CreateLogicalDevice()
{
	logicalDevice = logicaDeviceCreator->Create(physicalDevice, windowSurface, graphicsQueue, presentationQueue);
}

void VulkanRenderer::CreateWindowSurface() 
{
	windowSurface = windowsSurfaceCreator->CreateSurface(instance, *window);//surface is null, need to fix !
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

	return extensions;
}


void VulkanRenderer::DestroyInstance()
{
	vkDestroyInstance(instance, nullptr);
}
