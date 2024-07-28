#pragma once

#include <optional>
#include "EngineShared/IComponent.h"

struct MeshComponent : public IComponent
{
public:
	std::optional<int32_t> meshIndex;

	MeshComponent() {};
	MeshComponent(int32_t meshIndex) : meshIndex(meshIndex) {};
};
