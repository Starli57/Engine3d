#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "ValidationLayersInterface.h"

class InstanceInterface
{
public:
	void CreateInstance(VkInstance& instance) const;
	void DestroyInstance(VkInstance& instance) const;

private:
	std::vector<const char*> GetGLFWRequiredExtensions() const;
};

