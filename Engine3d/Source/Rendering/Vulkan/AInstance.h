#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "AValidationLayers.h"

namespace AVulkan
{
	class AInstance
	{
	public:
		void Create(VkInstance& instance) const;
		void Dispose(VkInstance& instance) const;

	private:
		std::vector<const char*> GetGLFWRequiredExtensions() const;
	};
}
