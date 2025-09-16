#pragma once

#include "Editor/Editor.h"
#include "EngineCore/EngineApi.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "Rendering/Vulkan/RendererVulkan.h"
#include "Serialization/EntitySerializer.h"
#include "Systems/CameraFlySystem.h"
#include "Systems/CameraLookAtSystem.h"
#include "Systems/DirectionalShadowMapProjectionSystem.h"
#include "Systems/RotateSystem.h"
#include "Systems/TransformSystem.h"
#include "Systems/ViewProjectionSystem.h"

class Game
{
public:
	Game(Ref<ProjectSettings> projectSettings);
	~Game();

	void Run() const;
private:
	Ref<Editor> editor;
	Ref<Engine::EngineApi> engine;
	Ref<Client::EntitySerializer> serializer;
	
	Ref<ProjectSettings> projectSettings;

	URef<CameraLookAtSystem> cameraLookAtPositionSystem;
	URef<RotateSystem> rotatorSystem;
	URef<TransformSystem> transformSystem;
	URef<ViewProjectionSystem> cameraProjectionSystem;
	URef<DirectionalShadowMapProjectionSystem> directionalShadowMapProjectionSystem;
	URef<CameraFlySystem> freeCameraSystem;

#if GLFW_INCLUDE_VULKAN
	Ref<ClientVulkanApi::RendererVulkan> renderer;
#endif
	
	void InitializeGameSystems();
	void UpdateGameSystems() const;
};

