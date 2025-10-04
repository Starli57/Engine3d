#include "EngineCore/Pch.h"
#include "VkInstanceWrapper.h"

#include "ValidationLayersUtility.h"
#include "EngineCore/CustomAssert.h"

namespace VulkanApi
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
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		const auto extensions = GetGLFWRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		SetupValidationLayers(createInfo);

		const VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
		Engine::CAssert::Check(result == VK_SUCCESS, "vulkan instance can't be created: " + result);
	}

	void DisposeInstance(const VkInstance& instance) 
	{
		spdlog::info("Dispose instance");
		vkDestroyInstance(instance, nullptr);
	}

	std::vector<const char*> GetGLFWRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef DEBUG
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

		spdlog::info("GLFW extensions included: {0}", extensions.size());

		return extensions;
	}
}
