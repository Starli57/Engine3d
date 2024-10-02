#pragma once

#include <filesystem>

#include "Asset.h"

class Texture : public Asset
{
public:
	const std::filesystem::path& textureFilePath;

	Texture(const std::filesystem::path& textureFilePath);
	virtual ~Texture();
};

