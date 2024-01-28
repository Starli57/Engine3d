#include "Pch.h"
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> ff)
{
	vertices.reserve(ff.size());

	for(int i = 0; i < ff.size(); i++)
	{
		vertices.push_back(ff.at(i));
	}
}

Mesh::~Mesh()
{
}

std::vector<Vertex> Mesh::GetVertices()
{
	return vertices;
}

uint32_t Mesh::GetVerticesCount()
{
	return vertices.size();
}
