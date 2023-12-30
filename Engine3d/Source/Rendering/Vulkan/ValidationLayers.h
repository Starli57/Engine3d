#pragma once

struct VkInstanceCreateInfo;

class ValidationLayers
{
public:
	void SetupValidationLayers(VkInstanceCreateInfo& createInfo);
};

