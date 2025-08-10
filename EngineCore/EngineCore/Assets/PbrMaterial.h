#pragma once

#include <string>
#include <optional>

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace EngineCore
{
	class PbrMaterial
	{
	public:
		std::string pipelineId;
		bool opaque = true;
	
		float roughness = 1.0f;
		float metallic = 0.0f;
		
		glm::vec4 baseColor = glm::vec4(1.0f);
		
		std::optional<uint32_t> baseTexture;
		std::optional<uint32_t> normalsTexture;
		std::optional<uint32_t> metallicRoughnessTexture;
		std::optional<uint32_t> occlusionTexture;
		std::optional<uint32_t> emissiveTexture;
		
		PbrMaterial() = default;
		PbrMaterial(std::string pipelineId);
	};
}
