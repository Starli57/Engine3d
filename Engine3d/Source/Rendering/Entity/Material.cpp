#include "Pch.h"
#include "Material.h"

Material::Material(const std::string& mainTexturePath)
{
	int width;
	int height;
	mainTexture = CreateRef<Texture>(mainTexturePath, width, height);
}

Material::~Material()
{
	mainTexture.reset();
}
