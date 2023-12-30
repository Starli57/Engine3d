#include "Pch.h"
#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer()
{
	validationLayers = new ValidationLayers();
	CreateInstance();
}

VulkanRenderer::~VulkanRenderer()
{
	DestroyInstance();
	delete validationLayers;
}

void VulkanRenderer::CreateInstance()
{
	VkApplicationInfo appInfo{};
	SetupAppInfo(appInfo);

	VkInstanceCreateInfo createInfo{};
	SetupInstanceCreateInfo(createInfo, appInfo);//todo: add errors handling
	validationLayers->SetupValidationLayers(createInfo);

	//todo: result can be success or not -> add errors handling
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

	//todo: glfwExtensions can be null -> add errors handling
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	createInfo.enabledLayerCount = 0;
}

void VulkanRenderer::DestroyInstance()
{
	vkDestroyInstance(instance, nullptr);
}
