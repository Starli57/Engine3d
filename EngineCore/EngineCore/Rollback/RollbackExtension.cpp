
#include "EngineCore/Pch.h"
#include "RollbackExtension.h"

namespace Engine
{
    void RollbackExtension::Dispose(const Rollback& rollback) const
    {
        Dispose(rollback.disposeStack);
    }

    void RollbackExtension::Dispose(const Rollback* rollback) const
    {
        Dispose(rollback->disposeStack);
    }

    void RollbackExtension::Dispose(std::stack<std::function<void()>>* disposeStack) const
    {
        while (disposeStack != nullptr && !disposeStack->empty())
        {
            auto dispose = disposeStack->top();
            dispose();
            disposeStack->pop();
        }
    }
}