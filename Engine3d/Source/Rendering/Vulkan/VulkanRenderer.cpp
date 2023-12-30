#include "Pch.h"
#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer()
{
	validationLayers = new ValidationLayers();
	physDeviceSelector = new PhysicalDeviceSelector();

	CreateInstance();
	SelectPhysicalRenderingDevice();


	//todo: delete after tests
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
	std::cout << "Rendering GPU: " << deviceProperties.deviceName << std::endl;
}

VulkanRenderer::~VulkanRenderer()
{
	DestroyInstance();

	delete physDeviceSelector;
	delete validationLayers;
}

void VulkanRenderer::CreateInstance()
{
	VkApplicationInfo appInfo{};
	SetupAppInfo(appInfo);

	VkInstanceCreateInfo createInfo{};
	SetupInstanceCreateInfo(createInfo, appInfo);
	validationLayers->SetupValidationLayers(createInfo);

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	if (result != VK_SUCCESS) std::cout << "vulkan instance can't be created: " << result;
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

	auto extensions = GetRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	createInfo.enabledLayerCount = 0;
}

void VulkanRenderer::SelectPhysicalRenderingDevice()
{
	physicalDevice = physDeviceSelector->GetBestRenderingDevice(instance);
}

std::vector<const char*> VulkanRenderer::GetRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions);
	return extensions;
}

void VulkanRenderer::DestroyInstance()
{
	vkDestroyInstance(instance, nullptr);
}
