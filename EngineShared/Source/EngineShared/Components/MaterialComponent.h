#pragma once

#include <string>

#include "Entities/Material.h"
#include "EngineShared/Ref.h"

class MaterialComponent
{
public:

	MaterialComponent(Ref<Material> material) : material(material)
	{
	}

	~MaterialComponent()
	{
		DestroyMaterial();
	}

	Ref<Material> GetMaterial()
	{
		return material;
	}

	void DestroyMaterial()
	{
		material.reset();
	}

private:
	Ref<Material> material;
};

