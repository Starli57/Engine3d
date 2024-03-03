#include "Pch.h"

#include "MeshContainer.h"

MeshContainer::MeshContainer()
{
}

MeshContainer::MeshContainer(Ref<Mesh> mesh, Ref<Material> material)
{
	this->mesh = mesh;
	this->material = material;
}

MeshContainer::~MeshContainer()
{
	DestroyMaterial();
	DestroyMesh();
}

void MeshContainer::UseMesh(Ref<Mesh> mesh)
{
	DestroyMesh();
	this->mesh = mesh;
}

void MeshContainer::UseMaterial(Ref<Material> material)
{
	DestroyMaterial();
	this->material = material;
}

void MeshContainer::DestroyMesh()
{
	mesh.reset();
}

void MeshContainer::DestroyMaterial()
{
	material.reset();
}

Ref<Mesh> MeshContainer::GetMesh()
{
	return mesh;
}

Ref<Material> MeshContainer::GetMaterial()
{
	return material;
}