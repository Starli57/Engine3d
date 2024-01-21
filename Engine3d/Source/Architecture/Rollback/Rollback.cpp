#include "Pch.h"

#include "Rollback.h"
#include "RollbackExtension.h"

#include "spdlog/spdlog.h"

Rollback::Rollback()
{
    disposeStack = new std::stack<std::function<void()>>();
}

Rollback::Rollback(Rollback& parentRollback)
{
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
    RollbackExtension().Dispose(disposeStack);
}