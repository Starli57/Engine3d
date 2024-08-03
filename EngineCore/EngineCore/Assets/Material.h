#pragma once

#include <string>
#include <optional>

#include "Texture.h"
#include "Asset.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Utilities/UniqueId.h"

class Material : public Asset
{
public:
	const std::string pipelineId;

	std::optional<uint32_t> albedoTexture;
	std::optional<uint32_t> normalMap;//not in use now
	std::optional<uint32_t> specular;//not in use now

	Material(const std::string pipelineId);
	~Material();

	void virtual SetAlbedoTexture(int32_t texture);
	void virtual SetNormalMap(int32_t texture);//not in use now
	void virtual SetSpecular(int32_t texture);//not in use now

	
};