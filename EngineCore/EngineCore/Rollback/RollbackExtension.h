#pragma once

#include <stack>
#include <functional>

#include "Rollback.h"

namespace EngineCore
{
	class RollbackExtension
	{
	public:
		void Dispose(const Rollback& rollback) const;
		void Dispose(const Rollback* rollback) const;
		void Dispose(std::stack<std::function<void()>>* disposeStack) const;

		friend class Rollback;
	};
}