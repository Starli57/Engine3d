#pragma once

struct VkInstanceCreateInfo;
struct VkDeviceCreateInfo;

class ValidationLayersInterface
{
public:
	void Setup(VkInstanceCreateInfo& createInfo);
	void Setup(VkDeviceCreateInfo& createInfo);
};

