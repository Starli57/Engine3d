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
}

Mesh* MeshRenderer::GetMesh()
{
	return mesh;
}
