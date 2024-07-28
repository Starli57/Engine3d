#pragma once

#include <string>

class MaterialComponent
{
public:
	uint32_t materialIndex;

	MaterialComponent() {}
	MaterialComponent(uint32_t materialIndex) : materialIndex(materialIndex) {};
};

