#include "Pch.h"

#include "Material.h"

Material::Material(Ref<Texture> texture)
{
	//todo: increment texture usage counter
	this->mainTexture = texture;
}

Material::~Material()
{
	//todo: decrement texture usage counter
}
