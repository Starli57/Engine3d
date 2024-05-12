#pragma once

class IEngineEditor
{
public:
	virtual void StartFrame() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};