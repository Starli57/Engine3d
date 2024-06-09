#pragma once

#include <iostream>

#include "EngineShared/IWindow.h"
#include "EngineShared/Ref.h"

class IEngineEditor
{
public:
	
	std::vector<Ref<IWindow>> windows;

	virtual void Update() = 0;

	void AddWindow(Ref<IWindow> window)
	{
		windows.push_back(window);
	}

	void RemoveWindow(Ref<IWindow> window)
	{
		windows.erase(std::remove(windows.begin(), windows.end(), window), windows.end());
	}
};