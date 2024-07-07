#pragma once

#include "EngineShared/IComponent.h"

struct MeshLoadRequest : public IComponent
{
	bool b;//without this variable there is error like 'retur ncannot convert from void to T todo: fix
};