
#include "Rollback.h"
#include "RollbackExtension.h"

#include "spdlog/spdlog.h"

Rollback::Rollback(const std::string& name) : name(name)
{
    disposeStack = new std::stack<std::function<void()>>();
}

Rollback::Rollback(const std::string& name, Rollback& parentRollback) : name(name)
{
    disposeStack = new std::stack<std::function<void()>>();
    parentRollback.Add([this]() { RollbackExtension().Dispose(this); });
}

Rollback::Rollback(const std::string& name, Ref<Rollback> parentRollback) : name(name)
{
    disposeStack = new std::stack<std::function<void()>>();
    parentRollback->Add([this]() { RollbackExtension().Dispose(this); });
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
    spdlog::info("Dispose rollback: " + name);
    RollbackExtension().Dispose(disposeStack);
    spdlog::info("Rollback disposed: " + name);
}