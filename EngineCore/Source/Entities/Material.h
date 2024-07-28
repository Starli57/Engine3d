#pragma once

#include <string>
#include <optional>

#include "Texture.h"
#include "EngineShared/Ref.h"

class Material
{
public:
	
	std::string pipelineId;

	int32_t albedoTexture = -1;
	int32_t normalMap = -1;
	int32_t specular = -1;

	Material(std::string pipelineId);
	~Material();

	void virtual SetAlbedoTexture(int32_t texture);
	void virtual SetNormalMap(int32_t texture);
	void virtual SetSpecular(int32_t texture);

	
};