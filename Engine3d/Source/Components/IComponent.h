#pragma once

class IComponent
{
public:
	virtual ~IComponent() {};
	virtual void Update() {};
};