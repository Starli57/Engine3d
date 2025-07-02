#pragma once

namespace EngineCore
{
	class IWindow
	{
	public:
		virtual ~IWindow() = default;
		virtual void Update() = 0;
	};
}