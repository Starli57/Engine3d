#pragma once

#include "Texture.h"
#include "AssetsDatabase.h"
#include "Architecture/Ref.h"

class Material
{
public:
	Material(Ref<AssetsDatabase> assetsDatabase, const std::string& mainTexturePath);
	~Material();

	Ref<Texture> mainTexture;
};