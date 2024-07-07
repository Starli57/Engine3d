#pragma once

#include "EngineShared/Ref.h"
#include "EngineShared/Mesh.h"
#include "EngineShared/IComponent.h"

struct MeshComponent : public IComponent
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
