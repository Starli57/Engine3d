#pragma once

#include <imgui.h>

#include "ComponentsRenderer.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Entity.h"
#include "EngineCore/Editor/IWindow.h"
#include "EngineCore/Core/ResourcesStorageVulkan.h"
using namespace EngineCore;

class Inspector : public IWindow
{
public:
	Inspector(Ref<ResourcesStorageVulkan> resourcesStorage);

	void Update() override;
	void Observe(const Ref<Entity>& entity);

private:
	URef<ComponentsRenderer> componentsRenderer;

	Ref<ResourcesStorageVulkan> resourcesStorage;
	Ref<Entity> observingEntity;

	void ShowAddComponentBtn() const;
};

