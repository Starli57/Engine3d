#include "Pch.h"
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>* vertices, std::vector<uint32_t>* indices)
{
	this->vertices = vertices;
	this->indices = indices;
}

Mesh::~Mesh()
{
	delete indices;
	delete vertices;
}

std::vector<Vertex>* Mesh::GetVertices()
{
	return vertices;
}

std::vector<uint32_t>* Mesh::GetIndices()
{
	return indices;
}

size_t Mesh::GetVerticesCount()
{
	return vertices->size();
}

size_t Mesh::GetIndicesCount()
{
	return indices->size();
}
