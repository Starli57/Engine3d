#pragma once

#include "Entities/Material.h"
#include "SharedLib/Ref.h"

class MaterialComponent
{
public:
	MaterialComponent(Ref<Material> material);
	~MaterialComponent();

	Ref<Material> GetMaterial();
	void DestroyMaterial();

private:
	Ref<Material> material;
};

