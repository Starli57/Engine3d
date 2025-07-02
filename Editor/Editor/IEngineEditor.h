#pragma once

#include <iostream>
#include <vector>

#include "EngineCore/Defines/DllDefines.h"
#include "EngineCore/Editor/IWindow.h"
#include "EngineCore/Core/Ref.h"

using namespace EngineCore;

class IEngineEditor
{
public:
	
	std::vector<Ref<IWindow>> windows;

	virtual void Update() = 0;

	void AddWindow(const Ref<IWindow>& window)
	{
		windows.push_back(window);
	}

	void RemoveWindow(const Ref<IWindow>& window)
	{
		windows.erase(std::remove(windows.begin(), windows.end(), window), windows.end());
	}
};