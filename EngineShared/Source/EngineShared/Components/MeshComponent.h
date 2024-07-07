#pragma once

#include "EngineShared/Mesh.h"
#include "EngineShared/Ref.h"

class MeshComponent
{
public:

	std::optional<std::filesystem::path> meshPath;

	MeshComponent() : meshPath(std::nullopt) 
	{
	}

	MeshComponent(std::filesystem::path& meshPath) : meshPath(meshPath) 
	{
	}

	~MeshComponent()
	{
		DestroyMesh();
	}

	void DestroyMesh()
	{
		mesh.reset();
	}

	void SetMesh(Ref<Mesh> mesh)
	{
		this->mesh = mesh;
	}

	Ref<Mesh> GetMesh()
	{
		return mesh;
	}

private:
	Ref<Mesh> mesh;
};
