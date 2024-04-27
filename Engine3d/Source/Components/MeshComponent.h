#pragma once

#include "Entities/Mesh.h"
#include "Entities/Material.h"
#include "SharedLib/Ref.h"

class MeshComponent
{
public:
	Ref<Mesh> GetMesh();

	MeshComponent(Ref<Mesh> mesh);
	~MeshComponent();

	void DestroyMesh();

private:
	Ref<Mesh> mesh;
};
