#pragma once

#include <stack>
#include <functional>
#include <unordered_map>

#include "Rollback.h"
#include "RollbackKey.h"

class RollbackMap
{
public:
	Rollback* GetRollback(const RollbackKey key);

	RollbackMap();
	~RollbackMap();

	Rollback* CreateRollback(const RollbackKey key);
	void Add(const RollbackKey key, std::function<void()> function);
	void Dispose(Rollback* rollback);

private:
	std::unordered_map<RollbackKey, Rollback*> disposeStackMap;
};

