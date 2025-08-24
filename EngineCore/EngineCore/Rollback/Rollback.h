#pragma once

#include <string>
#include <stack>
#include <functional>
#include "EngineCore/Core/Ref.h"

namespace Engine
{
	class RollbackExtension;

	class Rollback
	{
	public:
		Rollback(std::string name);
		Rollback(std::string name, const Rollback& parentRollback);
		Rollback(std::string name, const Ref<Rollback>& parentRollback);
		~Rollback();

		void Add(const std::function<void()>& function) const;
		void Dispose() const;

		friend class RollbackExtension;

	private:
		std::stack<std::function<void()>>* disposeStack;

		std::string name;
	};
}
