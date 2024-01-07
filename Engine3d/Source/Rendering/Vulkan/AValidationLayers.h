#pragma once

struct VkInstanceCreateInfo;
struct VkDeviceCreateInfo;

class AValidationLayers
{
public:
	void Setup(VkInstanceCreateInfo& createInfo);
	void Setup(VkDeviceCreateInfo& createInfo);
};

