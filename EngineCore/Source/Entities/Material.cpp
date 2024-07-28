#include "Pch.h"
#include "Material.h"

Material::Material(const std::string& pipelineId) : pipelineId(pipelineId)
{
}

Material::~Material()
{
}

void Material::SetAlbedoTexture(int32_t texture)
{
	this->albedoTexture = texture;
}

void Material::SetNormalMap(int32_t texture)
{
	this->normalMap = texture;
}

void Material::SetSpecular(int32_t texture)
{
	this->specular = texture;
}
