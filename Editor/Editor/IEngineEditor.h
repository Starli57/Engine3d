#pragma once

#include <iostream>
#include <vector>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Editor/IWindow.h"

class IEngineEditor
{
public:
	
	std::vector<Ref<Engine::IWindow>> windows;

	virtual void Update() = 0;

	void AddWindow(const Ref<Engine::IWindow>& window)
	{
		windows.push_back(window);
	}

	void RemoveWindow(const Ref<Engine::IWindow>& window)
	{
		windows.erase(std::remove(windows.begin(), windows.end(), window), windows.end());
	}
};
