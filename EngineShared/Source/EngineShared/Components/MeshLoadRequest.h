#pragma once

#include "EngineShared/IComponent.h"

struct MeshLoadRequest : public IComponent
{
	bool b;//without this variable there is error like 'return ncannot convert from void to T' todo: fix
};