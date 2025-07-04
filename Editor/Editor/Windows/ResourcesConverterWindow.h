#pragma once
#include "EngineCore/Editor/IWindow.h"

class ResourcesConverterWindow : public EngineCore::IWindow
{
public:
    ResourcesConverterWindow();
    void Update() override;

private:
    char inFolderPath[128];
    char outFolderePath[128];
};
