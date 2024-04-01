#pragma once

#include <GLFW/glfw3.h>
#include <entt.hpp>

#include "SharedLib/Ref.h"
#include "Rendering/Vulkan/VulkanModel.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "Rendering/Vulkan/Entities/MeshVulkan.h"
#include "Rendering/Vulkan/GraphicsPipeline.h"
#include "Systems/MeshComponent.h"
#include "SharedLib/Components/UboModelComponent.h"

namespace AVulkan
{
	class ACommandBuffer
	{
	public:
		void Setup(Ref<VulkanModel> model, int buffersCount) const;
		void Record(Ref<VulkanModel> model, Ref<entt::registry> ecs, VkFramebuffer& frameBuffer) const;
	};
}