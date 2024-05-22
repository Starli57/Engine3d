#pragma once

#include "Resources/TexturesList.h"

class Texture
{
public:
	const TextureId textureId;

	Texture(TextureId textureId);
	virtual ~Texture();
};

