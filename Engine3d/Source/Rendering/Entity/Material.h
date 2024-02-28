#pragma once

#include "Texture.h"
#include "Architecture/Ref.h"

class Material
{
public:
	Material(const std::string& mainTexturePath);
	~Material();

	Ref<Texture> mainTexture;
};