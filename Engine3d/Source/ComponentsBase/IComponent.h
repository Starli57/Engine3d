#pragma once

#include <string>

class IComponent
{
public:
	virtual std::string GetType() = 0;
	virtual void* GetComponent() = 0;
};