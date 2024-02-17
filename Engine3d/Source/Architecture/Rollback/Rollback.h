#pragma once

#include <stack>
#include <functional>
#include <string>

class RollbackExtension;

class Rollback
{
public:
	Rollback(std::string name);
	Rollback(std::string name, Rollback& parentRollback);
	~Rollback();

	void Add(std::function<void()> function);
	void Dispose();

	friend class RollbackExtension;

private:
	std::stack<std::function<void()>>* disposeStack;

	std::string name;
	bool isDisposed = false;
};

