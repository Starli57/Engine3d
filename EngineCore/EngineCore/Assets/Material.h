#pragma once

#include <string>
#include <optional>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Utilities/UniqueId.h"
#include "glm/vec3.hpp"

namespace EngineCore
{
	class Material
	{
	public:
		std::string pipelineId;
	
		float roughness = 1.0f;
		float metallic = 0.0f;
		float sheen = 0.5f;
		float specularExponent = 8.0f;
		float indexOfRefraction = 1.0f;
		float transparency = 1.0f;
            
		glm::vec3 ambientColor = glm::vec3(1.0f);
		glm::vec3 diffuseColor = glm::vec3(1.0f);
		glm::vec3 specularColor = glm::vec3(1.0f);
		glm::vec3 emissionColor = glm::vec3(0.0f);
	
		std::optional<uint32_t> diffuse;
		std::optional<uint32_t> specular;
		std::optional<uint32_t> normalMap;
		std::optional<uint32_t> alphaMap;

		bool opaque = true;

		Material() = default;
		Material(std::string pipelineId);

		void SetDiffuseTexture(uint32_t texture);
		void SetSpecular(uint32_t texture);
		void SetNormalMap(uint32_t texture);
		void SetAlphaMap(uint32_t texture);
	};
}