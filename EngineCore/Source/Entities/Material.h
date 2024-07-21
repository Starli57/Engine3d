#pragma once

#include <string>
#include <optional>

#include "Texture.h"
#include "EngineShared/Ref.h"

class Material
{
public:
	Material(std::string pipelineId);
	~Material();

	void virtual SetAlbedoTexture(Ref<Texture> texture);
	void virtual SetNormalMap(Ref<Texture> texture);
	void virtual SetSpecular(Ref<Texture> texture);

	std::optional<Ref<Texture>> albedoTexture;
	std::optional<Ref<Texture>> normalMap;
	std::optional<Ref<Texture>> specular;

	std::string pipelineId;
};