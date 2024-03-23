#include "Pch.h"
#include "MeshComponent.h"

MeshComponent::MeshComponent(Ref<Mesh> mesh, Ref<Material> material)
{
	this->mesh = mesh;
	this->material = material;
}

MeshComponent::~MeshComponent()
{
	DestroyMaterial();
	DestroyMesh();
}

void MeshComponent::DestroyMesh()
{
	mesh.reset();
}

void MeshComponent::DestroyMaterial()
{
	material.reset();
}

Ref<Mesh> MeshComponent::GetMesh()
{
	return mesh;
}

Ref<Material> MeshComponent::GetMaterial()
{
	return material;
}