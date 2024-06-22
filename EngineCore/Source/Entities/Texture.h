#pragma once

#include <filesystem>

class Texture
{
public:
	const std::filesystem::path& textureFilePath;

	Texture(std::filesystem::path& textureFilePath);
	virtual ~Texture();
};

