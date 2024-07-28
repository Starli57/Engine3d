#pragma once

#include <string>

#include "Entities/Material.h"
#include "EngineShared/Ref.h"

class MaterialComponent
{
public:
	uint32_t materialIndex;

	MaterialComponent(uint32_t materialIndex) : materialIndex(materialIndex) {};
};

