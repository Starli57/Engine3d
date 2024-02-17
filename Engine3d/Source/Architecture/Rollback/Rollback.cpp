#include "Pch.h"

#include "Rollback.h"
#include "RollbackExtension.h"

#include "spdlog/spdlog.h"

Rollback::Rollback(std::string name)
{
    this->name = name;
    disposeStack = new std::stack<std::function<void()>>();
}

Rollback::Rollback(std::string name, Rollback& parentRollback)
{
    this->name = name;
    disposeStack = new std::stack<std::function<void()>>();
    parentRollback.Add([this]() { RollbackExtension().Dispose(disposeStack); });
}

Rollback::~Rollback()
{
    Dispose();
    delete disposeStack;
}

void Rollback::Add(std::function<void()> function)
{
    disposeStack->push(function);
}

void Rollback::Dispose()
{
    if (isDisposed)
    {
        spdlog::critical("Rollback already disposed: " + name);
        return;
    }

    RollbackExtension().Dispose(disposeStack);
    isDisposed = true;
}