
#include "EngineCore/Pch.h"

#include "Rollback.h"
#include "RollbackExtension.h"

#include "spdlog/spdlog.h"

Rollback::Rollback(std::string name) : name(std::move(name))
{
    disposeStack = new std::stack<std::function<void()>>();
}

Rollback::Rollback(std::string name, const Rollback& parentRollback) : name(std::move(name))
{
    disposeStack = new std::stack<std::function<void()>>();
    parentRollback.Add([this]() { RollbackExtension().Dispose(this); });
}

Rollback::Rollback(std::string name, const Ref<Rollback>& parentRollback) : name(std::move(name))
{
    disposeStack = new std::stack<std::function<void()>>();
    parentRollback->Add([this]() { RollbackExtension().Dispose(this); });
}

Rollback::~Rollback()
{
    Dispose();
    delete disposeStack;
}

void Rollback::Add(const std::function<void()>& function) const
{
    disposeStack->push(function);
}

void Rollback::Dispose() const
{
    spdlog::info("Dispose rollback: " + name);
    RollbackExtension().Dispose(disposeStack);
    spdlog::info("Rollback disposed: " + name);
}
