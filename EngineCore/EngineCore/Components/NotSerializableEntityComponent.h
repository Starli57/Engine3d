#pragma once
#include "EngineCore/Core/IComponent.h"

struct NotSerializableEntityComponent : public IComponent
{
    /*[NotSerializable]*/bool noFields;
};
