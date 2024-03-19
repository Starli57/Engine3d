#pragma once

#include "ISystem.h"
#include "Entities/Mesh.h"
#include "Entities/Material.h"
#include "SharedLib/Ref.h"

class MeshContainer : public ISystem
{
public:

	Ref<Mesh> GetMesh();
	Ref<Material> GetMaterial();

	MeshContainer();
	MeshContainer(Ref<Mesh> mesh, Ref<Material> material);
	virtual ~MeshContainer() override;

	void UseMesh(Ref<Mesh> mesh);
	void UseMaterial(Ref<Material> mateial);

	void DestroyMesh();
	void DestroyMaterial();

private:
	Ref<Mesh> mesh;
	Ref<Material> material;
};