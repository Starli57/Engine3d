#include "Pch.h"
#include "Rollback.h"
#include "spdlog/spdlog.h"

Rollback::Rollback()
{
    disposeStack = new std::stack<std::function<void()>>();
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
    while (disposeStack != nullptr && !disposeStack->empty())
    {
        auto dispose = disposeStack->top();
        dispose();
        disposeStack->pop();
    }
}
