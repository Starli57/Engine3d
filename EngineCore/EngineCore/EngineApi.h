#pragma once

#include <vector>
#include <chrono>
#include <entt/entt.hpp>

#include "EngineContext.h"
#include "Core/SystemsState.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Managers/InputManager.h"
#include "EngineCore/Core/ProjectSettings.h"

#include "EngineCore/Rollback/Rollback.h"
#include "Serialization/EntitySerializer.h"

#if GLFW_INCLUDE_VULKAN
#include "EngineCore/Core/ResourcesStorageVulkan.h"
#include "EngineCore/Core/AssetsLoaderVulkan.h"
#include "EngineCore/Rendering/Vulkan/RendererVulkan.h"
#endif

namespace Engine
{
	class EngineApi
	{
	public:
		EngineApi(const Ref<ProjectSettings>& projectSettings);
		virtual ~EngineApi();
	
		void InitLogger() const;
		void InitGlfw() const;
		void SetupGlfwHints() const;
		void CreateAppWindow();
		void DefineInput();
		void DefineGraphicsApi();
		void DefineResourcesManager();
	
		void Run();

		void BindGameSystemsUpdateFunction(std::function<void()> gameSystemsUpdate);
		void BindEditorUpdateFunction(std::function<void()> editorUpdate);

		float GetDeltaTime() const { return deltaTime; }
		
		EngineContext* engineContext;
		
	private:
		float deltaTime;

		std::function<void()> gameSystemsUpdate;
		std::function<void()> editorUpdate;

		std::chrono::steady_clock::time_point cachedTime;

	};
}