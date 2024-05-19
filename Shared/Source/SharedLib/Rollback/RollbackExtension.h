#pragma once

#include <stack>
#include <functional>

#include "Rollback.h"

class RollbackExtension
{
public:
	void Dispose(Rollback rollback);
	void Dispose(Rollback* rollback);
	void Dispose(std::stack<std::function<void()>>* disposeStack);

	friend class Rollback;
};

