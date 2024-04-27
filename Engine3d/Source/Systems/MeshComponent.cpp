#include "Pch.h"
#include "MeshComponent.h"

MeshComponent::MeshComponent(Ref<Mesh> mesh)
{
	this->mesh = mesh;
}

MeshComponent::~MeshComponent()
{
	DestroyMesh();
}

void MeshComponent::DestroyMesh()
{
	mesh.reset();
}

Ref<Mesh> MeshComponent::GetMesh()
{
	return mesh;
}
