#include "Pch.h"
#include "Rollback.h"
#include "spdlog/spdlog.h"

Rollback::Rollback()
{
    disposeStack = new std::stack<std::function<void()>>();
}

Rollback::Rollback(Rollback& parentRollback)
{
    disposeStack = new std::stack<std::function<void()>>();
    parentRollback.Add([this]() { Dispose(disposeStack); });
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
    Dispose(disposeStack);
}

void Rollback::Dispose(Rollback rollback)
{
    Dispose(rollback.disposeStack);
}

void Rollback::Dispose(std::stack<std::function<void()>>* dstack)
{
    while (dstack != nullptr && !dstack->empty())
    {
        auto dispose = dstack->top();
        dispose();
        dstack->pop();
    }
}
