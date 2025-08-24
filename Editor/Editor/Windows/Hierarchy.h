#pragma once

#include <imgui.h>

#include "Inspector.h"

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "EngineCore/Editor/IWindow.h"
#include "EngineCore/Components/NameComponent.h"
#include "EngineCore/Components/IdComponent.h"
#include "EngineCore/Serialization/EntitySerializer.h"

class Hierarchy : public IWindow
{
public:
	Hierarchy(const Ref<EngineCore::EntitySerializer>& serializer, const Ref<Ecs>& ecs, const Ref<Inspector>& inspector, const Ref<ProjectSettings>& projectSettings, const
	          Ref<ResourcesStorageVulkan>& resourcesStorage);
	~Hierarchy();

	void Update() override;

private:
	Ref<Ecs> ecs;
	Ref<Inspector> inspector;
	Ref<EngineCore::EntitySerializer> serializer;
	Ref<ProjectSettings> projectSettings;
	Ref<ResourcesStorageVulkan> resourcesStorage;

	int selectedItemIndex;
};

