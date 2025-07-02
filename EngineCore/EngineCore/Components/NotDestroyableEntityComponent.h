#pragma once
#include "EngineCore/Core/IComponent.h"

struct NotDestroyableEntityComponent : public IComponent
{
    /*[NotSerializable]*/bool noFields;
};
