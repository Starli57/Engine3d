#pragma once

#include <string>

struct MaterialComponent
{
	uint32_t materialIndex;

	MaterialComponent() { materialIndex = 0; }
	MaterialComponent(const uint32_t materialIndex) : materialIndex(materialIndex) {}
};

