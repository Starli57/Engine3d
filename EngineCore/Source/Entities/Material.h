#pragma once

#include "Texture.h"
#include "EngineShared/Ref.h"

class Material
{
public:
	Material(Ref<Texture> texture);
	~Material();

	Ref<Texture> mainTexture;
};