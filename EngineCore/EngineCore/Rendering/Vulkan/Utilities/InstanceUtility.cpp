#include "EngineCore/Pch.h"
#include "InstanceUtility.h"

namespace VkUtils
{
	void CreateInstance(VkInstance& instance)
	{
		spdlog::info("Create Instance");

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

		VkUtils::SetupValidationLayers(createInfo);

		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
		CAssert::Check(result == VK_SUCCESS, "vulkan instance can't be created: " + result);
	}

	void DisposeInstance(VkInstance& instance) 
	{
		spdlog::info("Dispose instance");
		vkDestroyInstance(instance, nullptr);
	}

	std::vector<const char*> GetGLFWRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef DEBUG
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

		spdlog::info("GLFW extesions included: {0}", extensions.size());

		return extensions;
	}
}