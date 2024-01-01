#include "Pch.h"
#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer()
{
	validationLayers = new ValidationLayers();
	physycalDeviceSelector = new PhysicalDeviceSelector();
	logicaDeviceCreator = new LogicalDeviceCreator();

	CreateInstance();
	SelectPhysicalRenderingDevice();
	CreateLogicalDevice();

	//todo: delete after tests
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
	std::cout << "Rendering GPU: " << deviceProperties.deviceName << std::endl;
}

VulkanRenderer::~VulkanRenderer()
{
	vkDestroyDevice(logicalDevice, nullptr);
	delete logicaDeviceCreator;
	delete physycalDeviceSelector;
	delete validationLayers;
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
	validationLayers->Setup(createInfo);

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	if (result != VK_SUCCESS) std::cout << "vulkan instance can't be created: " << result;//todo: add errors handling
}

void VulkanRenderer::SelectPhysicalRenderingDevice()
{
	physicalDevice = physycalDeviceSelector->GetBestRenderingDevice(instance);
}

void VulkanRenderer::CreateLogicalDevice()
{
	logicalDevice = logicaDeviceCreator->Create(physicalDevice);
}

std::vector<const char*> VulkanRenderer::GetGLFWRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions = std::vector<const char*>();
	for (int i = 0; i < glfwExtensionCount; i++) extensions.push_back(glfwExtensions[i]);

#ifdef DEBUG
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

	std::cout << "GLFW extensions count: " << glfwExtensionCount << std::endl;

	return extensions;
}


void VulkanRenderer::DestroyInstance()
{
	vkDestroyInstance(instance, nullptr);
}
