#pragma once

#include "SharedLib/Ref.h"
#include "UboModel.h"
#include "Renderer/Entities/Mesh.h"

struct RenderModel
{
public:
	UboModel& uboModel;
	Ref<Mesh> mesh;

	RenderModel(UboModel& uboModel, Ref<Mesh> mesh) : uboModel(uboModel), mesh(mesh)
	{
	}
};