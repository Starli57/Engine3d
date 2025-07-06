#pragma once

#include "Editor/Editor.h"
#include "EngineCore/Engine.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "Rendering/Vulkan/RendererVulkan.h"
#include "Serialization/EntitySerializer.h"
#include "Systems/CameraFlySystem.h"
#include "Systems/CameraLookAtSystem.h"
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
	Ref<Engine> engine;
	Ref<Client::EntitySerializer> serializer;
	
	Ref<ProjectSettings> projectSettings;
\
	URef<CameraLookAtSystem> cameraLookAtPositionSystem;\
	URef<RotateSystem> rotatorSystem;
	URef<TransformSystem> transformSystem;
	URef<ViewProjectionSystem> cameraSystem;
	URef<CameraFlySystem> freeCameraSystem;\

#if GLFW_INCLUDE_VULKAN
	Ref<AVulkan::RendererVulkan> renderer;
#endif
	
	void InitializeGameSystems();
	void UpdateGameSystems() const;
};

