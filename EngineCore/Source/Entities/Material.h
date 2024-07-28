#pragma once

#include <string>
#include <optional>

#include "Asset.h"
#include "Texture.h"
#include "EngineShared/Ref.h"
#include "EngineShared/UniqueId.h"

class Material : public Asset
{
public:
	std::string pipelineId;

	int32_t albedoTexture = -1;
	int32_t normalMap = -1;//not in use now
	int32_t specular = -1;//not in use now

	Material(const std::string& pipelineId);
	~Material();

	void virtual SetAlbedoTexture(int32_t texture);
	void virtual SetNormalMap(int32_t texture);//not in use now
	void virtual SetSpecular(int32_t texture);//not in use now

	
};