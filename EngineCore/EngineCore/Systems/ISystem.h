#pragma once

class ISystem
{
public:
	virtual ~ISystem() {}
	virtual void Update(float deltaTime = 0) {}
};