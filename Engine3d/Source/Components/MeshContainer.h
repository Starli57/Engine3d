#pragma once

#include "Rendering/Model/Mesh.h"
#include "Architecture/Ref.h"

class MeshContainer
{
public:

	Ref<Mesh> GetMesh();

	MeshContainer();
	MeshContainer(Ref<Mesh> mesh);
	~MeshContainer();

	void UseMesh(Ref<Mesh> mesh);
	void DestroyMesh();

private:
	Ref<Mesh> mesh;
};
