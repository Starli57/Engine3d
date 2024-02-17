#pragma once

#include <string>
#include <stack>
#include <functional>

class RollbackExtension;

class Rollback
{
public:
	Rollback(const std::string& name);
	Rollback(const std::string& name, Rollback& parentRollback);
	~Rollback();

	void Add(std::function<void()> function);
	void Dispose();

	friend class RollbackExtension;

private:
	std::stack<std::function<void()>>* disposeStack;

	std::string name;
};

