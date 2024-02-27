#pragma once

#include <GLFW/glfw3.h>
#include <entt.hpp>

#include "Architecture/Ref.h"
#include "Rendering/Vulkan/Mesh/MeshVulkan.h"
#include "Rendering/Vulkan/VulkanContext.h"
#include "Components/MeshContainer.h"
#include "Components/Transform.h"

namespace AVulkan
{
	class ACommandBuffer
	{
	public:
		void Setup(Ref<VulkanContext> vulkanContext, int buffersCount) const;
		void Record(Ref<entt::registry> ecs, Ref<VulkanContext> vulkanContext, uint32_t imageIndex) const;
	};
}