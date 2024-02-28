#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <entt.hpp>

#include "Entity.h"
#include "Architecture/Rollback/Rollback.h"
#include "Rendering/Model/Vertex.h"
#include "Rendering/Entity/Mesh.h"
#include "Components/Camera.h"
#include "Rendering/IGraphicsApi.h"
#include "Rendering/Vulkan/AssetsDatabaseVulkan.h"

class Level
{
public:
	Level(Ref<entt::registry> ecs, Ref<AVulkan::AssetsDatabaseVulkan> assetDatabase, IGraphicsApi* graphicsApi, Rollback* rollback);
	~Level();

	void LoadLevel();
	void UnloadLevel();

private:
	Ref<entt::registry> ecs;
	Ref<AVulkan::AssetsDatabaseVulkan> assetDatabase;

	IGraphicsApi* graphicsApi;
	Rollback* rollback;

};

