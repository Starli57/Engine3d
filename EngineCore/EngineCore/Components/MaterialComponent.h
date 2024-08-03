#pragma once

#include <string>

class MaterialComponent
{
public:
	uint32_t materialIndex;

	MaterialComponent() { materialIndex = 0; }
	MaterialComponent(uint32_t materialIndex) : materialIndex(materialIndex) {};
};

