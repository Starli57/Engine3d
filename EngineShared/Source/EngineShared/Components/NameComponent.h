#pragma once

#include <string>
#include "EngineShared/IComponent.h"

struct NameComponent : public IComponent
{
	std::string name;

	NameComponent() { name = "<NONE>"; }
	NameComponent(std::string name) :name(name) {}
};