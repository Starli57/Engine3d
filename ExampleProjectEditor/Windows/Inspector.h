#pragma once

#include <imgui.h>

#include "ComponentsRenderer.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Entity.h"
#include "EngineCore/Editor/IWindow.h"
#include "EngineCore/Core/ResourcesStorageVulkan.h"

namespace UserEditor
{
	class Inspector : public Engine::IWindow
	{
	public:
		Inspector(Ref<Engine::ResourcesStorageVulkan> resourcesStorage);

		void Update() override;
		void Observe(const Ref<Entity>& entity);

	private:
		URef<ComponentsRenderer> componentsRenderer;

		Ref<Engine::ResourcesStorageVulkan> resourcesStorage;
		Ref<Entity> observingEntity;

		void ShowAddComponentBtn() const;
	};
}