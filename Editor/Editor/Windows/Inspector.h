#pragma once

#include <imgui.h>

#include "ComponentsRenderer.h"
#include "EngineCore/Ref.h"
#include "EngineCore/Entity.h"
#include "EngineCore/IWindow.h"
#include "EngineCore/AssetsDatabase.h"

class Inspector : public IWindow
{
public:
	Inspector(Ref<AssetsDatabase> assetsDatabase);

	void Update() override;
	void Observe(Ref<Entity> entity);

private:
	URef<ComponentsRenderer> componentsRenderer;

	Ref<AssetsDatabase> assetsDatabase;
	Ref<Entity> observingEntity;

	void ShowAddComponentBtn();
};

