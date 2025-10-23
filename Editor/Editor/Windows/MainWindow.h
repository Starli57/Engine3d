#pragma once
#include "EngineCore/Editor/IWindow.h"

namespace Editor
{
    class MainWindow : public Engine::IWindow
    {
    public:
        void Update() override;
    };
}