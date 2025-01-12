
#include "EngineCore/Pch.h"
#include "EngineCore/Assets/Material.h"

Material::Material(const std::string pipelineId) : pipelineId(pipelineId)
{
}

void Material::SetDiffuseTexture(uint32_t texture)
{
	this->diffuse = texture;
}

void Material::SetSpecular(uint32_t texture)
{
	this->specular = texture;
}

void Material::SetNormalMap(uint32_t texture)
{
	this->normalMap = texture;
}

void Material::SetAlphaMap(uint32_t texture)
{
	this->alphaMap = texture;
}
