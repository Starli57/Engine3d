#include "Pch.h"
#include "RollbackExtension.h"


void RollbackExtension::Dispose(Rollback rollback)
{
    Dispose(rollback.disposeStack);
}

void RollbackExtension::Dispose(std::stack<std::function<void()>>* disposeStack)
{
    while (disposeStack != nullptr && !disposeStack->empty())
    {
        auto dispose = disposeStack->top();
        dispose();
        disposeStack->pop();
    }
}
