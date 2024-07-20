#include "Pch.h"
#include "Material.h"

Material::Material(Ref<Texture> texture, std::string pipelineId) : mainTexture(texture), pipelineId(pipelineId)
{
	//todo: increment texture usage counter
}

Material::~Material()
{
	//todo: decrement texture usage counter
}
