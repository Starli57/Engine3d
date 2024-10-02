#pragma once

#include <string>
#include "EngineCore/Core/IComponent.h"
#include "EngineCore/Utilities/UniqueId.h"

struct IdComponent : public IComponent
{
	const uint32_t id;

	IdComponent() : id(UniqueIdGenerator::Generate()) {}
	IdComponent(uint32_t id) : id(id) {}

};