#pragma once

#include <stack>
#include <functional>


class Rollback
{
public:
	Rollback();
	Rollback(Rollback& parentRollback);

	void Dispose();
	void Dispose(Rollback rollback);

	void Add(std::function<void()> function);

	~Rollback();

private:
	std::stack<std::function<void()>>* disposeStack;

	void Dispose(std::stack<std::function<void()>>* stack);
};

