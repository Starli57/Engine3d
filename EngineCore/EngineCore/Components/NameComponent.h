#pragma once

#include <string>
#include "EngineCore/Core/IComponent.h"

struct NameComponent : public IComponent
{
	std::string name;

	NameComponent() { name = "<NONE>"; }
	NameComponent(std::string name) :name(name) {}
};