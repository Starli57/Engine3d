#pragma once

struct VkInstanceCreateInfo;
struct VkDeviceCreateInfo;

class ValidationLayers
{
public:
	void Setup(VkInstanceCreateInfo& createInfo);
	void Setup(VkDeviceCreateInfo& createInfo);
};

