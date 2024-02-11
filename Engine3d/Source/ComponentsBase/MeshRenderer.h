#pragma once

#include <string>

#include "IComponent.h"
#include "Rendering/Model/Mesh.h"

class MeshRenderer : public IComponent
{
public:
	MeshRenderer();
	MeshRenderer(Mesh* mesh);
	~MeshRenderer();

	Mesh* GetMesh();

	std::string GetType() override { return type; }
	void* GetComponent() override { return this; }

private:
	Mesh* mesh;

	const std::string type = "MeshRenderer";
};

