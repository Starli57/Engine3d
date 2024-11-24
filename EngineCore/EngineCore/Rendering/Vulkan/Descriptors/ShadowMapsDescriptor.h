#pragma once

#include "GLFW/glfw3.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/Models/BufferModel.h"

namespace AVulkan
{
	struct ShadowMapsDescriptor
	{
		VkDescriptorSet descriptorSet;

		Ref<BufferModel> uboLightsViewProjection;
	};
}