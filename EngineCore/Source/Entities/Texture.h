#pragma once

#include <filesystem>

#include "Asset.h"
#include "EngineShared/UniqueId.h"

class Texture : public Asset
{
public:
	const std::filesystem::path& textureFilePath;

	Texture(std::filesystem::path& textureFilePath);
	virtual ~Texture();
};

