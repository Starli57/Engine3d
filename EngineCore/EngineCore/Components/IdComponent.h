#pragma once

#include <string>
#include "EngineCore/Core/IComponent.h"
#include "EngineCore/Utilities/UniqueId.h"

using namespace EngineCore;

struct IdComponent : public IComponent
{
	const uint32_t id;

	IdComponent() : id(UniqueIdGenerator::Generate()) {}
	IdComponent(const uint32_t id) : id(id) {}

};