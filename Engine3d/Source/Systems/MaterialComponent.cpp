#include "Pch.h"
#include "MaterialComponent.h"

MaterialComponent::MaterialComponent(Ref<Material> material) : material(material)
{
}

MaterialComponent::~MaterialComponent()
{
	DestroyMaterial();
}

Ref<Material> MaterialComponent::GetMaterial()
{
	return material;
}

void MaterialComponent::DestroyMaterial()
{
	material.reset();
}
