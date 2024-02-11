#include "Pch.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::MeshRenderer(Mesh* mesh)
{
	this->mesh = mesh;
}

MeshRenderer::~MeshRenderer()
{
	throw std::runtime_error("Not implemented");
}

void MeshRenderer::AddMesh(Mesh* mesh)
{
	DestroyMesh();
	this->mesh = mesh;
}

void MeshRenderer::DestroyMesh()
{
	throw std::runtime_error("Not implemented");
}

Mesh* MeshRenderer::GetMesh()
{
	return mesh;
}
