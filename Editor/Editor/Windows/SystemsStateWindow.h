#pragma once
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/SystemsState.h"
#include "EngineCore/Editor/IWindow.h"

class SystemsStateWindow : public Engine::IWindow
{
public:
    SystemsStateWindow(const Ref<SystemsState>& systemsState);
    void Update() override;

private:
    Ref<SystemsState> systemsState;
};
