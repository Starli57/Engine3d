#include "Pch.h"
#include "Material.h"

Material::Material(std::string pipelineId) : pipelineId(pipelineId)
{
}

Material::~Material()
{
}

void Material::SetAlbedoTexture(Ref<Texture> texture)
{
	this->albedoTexture = texture;
}

void Material::SetNormalMap(Ref<Texture> texture)
{
	this->normalMap = texture;
}

void Material::SetSpecular(Ref<Texture> texture)
{
	this->specular = texture;
}
