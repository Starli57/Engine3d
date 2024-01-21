#pragma once

#include <stack>
#include <functional>

class RollbackExtension;

class Rollback
{
public:
	Rollback();
	Rollback(Rollback& parentRollback);
	~Rollback();

	void Add(std::function<void()> function);
	void Dispose();

	friend class RollbackExtension;

private:
	std::stack<std::function<void()>>* disposeStack;
};

