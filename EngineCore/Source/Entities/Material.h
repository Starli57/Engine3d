#pragma once

#include <string>

#include "Texture.h"
#include "EngineShared/Ref.h"

class Material
{
public:
	Material(Ref<Texture> texture, std::string pipelineId);
	~Material();

	Ref<Texture> mainTexture;
	std::string pipelineId;
};