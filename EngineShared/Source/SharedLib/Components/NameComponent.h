#pragma once

#include <iostream>

struct NameComponent
{
	std::string name;

	NameComponent() { name = ""; }
	NameComponent(std::string name) :name(name) {}
};