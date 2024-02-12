#include "Pch.h"
#include "MeshContainer.h"

MeshContainer::MeshContainer()
{
}

MeshContainer::MeshContainer(Ref<Mesh> mesh)
{
	this->mesh = mesh;
}

MeshContainer::~MeshContainer()
{
	DestroyMesh();
}

void MeshContainer::UseMesh(Ref<Mesh> mesh)
{
	DestroyMesh();
	this->mesh = mesh;
}

void MeshContainer::DestroyMesh()
{
	mesh.reset();
}

Ref<Mesh> MeshContainer::GetMesh()
{
	return mesh;
}