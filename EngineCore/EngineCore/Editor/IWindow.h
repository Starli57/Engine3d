#pragma once

namespace Engine
{
	class IWindow
	{
	public:
		virtual ~IWindow() = default;
		virtual void Update() = 0;
	};
}