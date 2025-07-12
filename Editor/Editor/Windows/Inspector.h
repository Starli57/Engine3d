#pragma once

#include <imgui.h>

#include "ComponentsRenderer.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Entity.h"
#include "EngineCore/Editor/IWindow.h"
#include "EngineCore/Core/ResourcesStorage.h"
using namespace EngineCore;

class Inspector : public IWindow
{
public:
	Inspector(Ref<ResourcesStorage> assetsDatabase);

	void Update() override;
	void Observe(const Ref<Entity>& entity);

private:
	URef<ComponentsRenderer> componentsRenderer;

	Ref<ResourcesStorage> assetsDatabase;
	Ref<Entity> observingEntity;

	void ShowAddComponentBtn() const;
};

