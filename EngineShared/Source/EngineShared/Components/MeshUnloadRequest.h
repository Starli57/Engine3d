#pragma once

#include "EngineShared/IComponent.h"

struct MeshUnloadRequest : public IComponent
{
	bool b;//without this variable there is error like 'return ncannot convert from void to T' todo: fix
};