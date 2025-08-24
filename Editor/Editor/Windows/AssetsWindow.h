#pragma once

#include <imgui.h>
#include "EngineCore/Editor/IWindow.h"
#include "EngineCore/Core/ResourcesStorageVulkan.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Serialization/EntitySerializer.h"

class AssetsWindow : public Engine::IWindow
{
public:
	AssetsWindow(Ref<Engine::EntitySerializer> serializer, const Ref<Ecs>& ecs, const Ref<Engine::ResourcesStorageVulkan>& resourcesStorage, const Ref<ProjectSettings>& projectSettings);
	~AssetsWindow();

	void Update() override;

private:
	Ref<Ecs> ecs;
	Ref<Engine::ResourcesStorageVulkan> resourcesStorage;
	Ref<ProjectSettings> projectSettings;
	Ref<Engine::EntitySerializer> serializer;
};

