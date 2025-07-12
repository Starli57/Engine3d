#pragma once

#include <imgui.h>
#include "EngineCore/Editor/IWindow.h"
#include "EngineCore/Core/ResourcesStorage.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Serialization/EntitySerializer.h"

class AssetsWindow : public IWindow
{
public:
	AssetsWindow(Ref<EngineCore::EntitySerializer> serializer, const Ref<Ecs>& ecs, const Ref<ResourcesStorage>& assetsDatabase, const Ref<ProjectSettings>& projectSettings);
	~AssetsWindow();

	void Update() override;

private:
	Ref<Ecs> ecs;
	Ref<ResourcesStorage> assetsDatabase;
	Ref<ProjectSettings> projectSettings;
	Ref<EngineCore::EntitySerializer> serializer;
};

