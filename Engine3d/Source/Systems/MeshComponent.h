#pragma once

#include "ISystem.h"
#include "Entities/Mesh.h"
#include "Entities/Material.h"
#include "SharedLib/Ref.h"

class MeshComponent
{
public:
	Ref<Mesh> GetMesh();
	Ref<Material> GetMaterial();

	MeshComponent(Ref<Mesh> mesh, Ref<Material> material);
	~MeshComponent();

	void DestroyMesh();
	void DestroyMaterial();

private:
	Ref<Mesh> mesh;
	Ref<Material> material;
};
