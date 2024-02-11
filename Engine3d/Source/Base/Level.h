#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Architecture/Rollback/Rollback.h"
#include "Rendering/Model/Vertex.h"
#include "Rendering/Model/Mesh.h"
#include "BaseComponents/Camera.h"

class Level
{
public:
	std::vector<Mesh*>* GetMeshes();
	Camera* GetCamera();

	Level(Rollback* rollback);
	~Level();

	void LoadLevel();
	void UnloadLevel();

private:
	Rollback* rollback;

	std::vector<Mesh*>* meshes;
	Camera* camera;
};

