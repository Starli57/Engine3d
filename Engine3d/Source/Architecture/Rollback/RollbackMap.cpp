#include "Pch.h"
#include "RollbackMap.h"
#include "spdlog/spdlog.h"

Rollback* RollbackMap::GetRollback(const RollbackKey key)
{
    auto rollbackIt = disposeStackMap.find(key);
    if (rollbackIt == disposeStackMap.end())
        throw std::runtime_error("Rollback not found " + static_cast<int>(key));
    return nullptr;
}

RollbackMap::RollbackMap()
{
}

RollbackMap::~RollbackMap()
{
    for (auto i = disposeStackMap.begin(); i != disposeStackMap.end(); i++)
    {
        Dispose(i->second);
    }
}

Rollback* RollbackMap::CreateRollback(const RollbackKey key)
{
    auto rollbackIt = disposeStackMap.find(key);
    if (rollbackIt != disposeStackMap.end())
        spdlog::warn("Rollback with the same key already created: {0}", static_cast<int>(key));

    Rollback* rollback = new Rollback("");
    disposeStackMap[key] = rollback;
    return rollback;
}

void RollbackMap::Add(const RollbackKey key, std::function<void()> function)
{
    auto rollbackIt = disposeStackMap.find(key);
    if (rollbackIt == disposeStackMap.end())
    {
        Rollback* rollback = new Rollback("");
        rollback->Add(function);
        disposeStackMap[key] = rollback;
    }
    else
    {
        rollbackIt->second->Add(function);
    }
}

void RollbackMap::Dispose(Rollback* rollback)
{
    rollback->Dispose();
    delete rollback;
}
