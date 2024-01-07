#pragma once

#include <stack>
#include <functional>


class Rollback
{
public:
	Rollback();
	~Rollback();

	void Add(std::function<void()> function);
	void Dispose();

private:
	std::stack<std::function<void()>>* disposeStack;
};

