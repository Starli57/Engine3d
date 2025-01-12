#pragma once

#include <string>
#include <optional>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Utilities/UniqueId.h"

class Material
{
public:
	const std::string pipelineId;

	std::optional<uint32_t> diffuse;
	std::optional<uint32_t> specular;
	std::optional<uint32_t> normalMap;
	std::optional<uint32_t> alphaMap;

	Material(const std::string pipelineId);

	void virtual SetDiffuseTexture(uint32_t texture);
	void virtual SetSpecular(uint32_t texture);
	void virtual SetNormalMap(uint32_t texture);
	void virtual SetAlphaMap(uint32_t texture);
	
};