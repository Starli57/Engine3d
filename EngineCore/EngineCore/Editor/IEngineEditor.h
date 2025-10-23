#pragma once

#include <iostream>
#include <vector>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Editor/IWindow.h"

namespace Engine
{
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
}