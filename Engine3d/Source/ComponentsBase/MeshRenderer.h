#pragma once

#include "Rendering/Model/Mesh.h"

class MeshRenderer
{
public:
	MeshRenderer();
	MeshRenderer(Mesh* mesh);
	~MeshRenderer();

	Mesh* GetMesh();

private:
	Mesh* mesh;
};

