#pragma once

#include <iostream>

#include "SharedLib/IWindow.h"
#include "SharedLib/Ref.h"

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