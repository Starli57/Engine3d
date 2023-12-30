#pragma once

#include "ValidationLayers.h"

class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

private:
	VkInstance instance;
	ValidationLayers* validationLayers;

	void CreateInstance();
	void DestroyInstance();

	void SetupAppInfo(VkApplicationInfo& info);
	bool SetupInstanceCreateInfo(VkInstanceCreateInfo& createInfo, VkApplicationInfo& appInfo);

};

