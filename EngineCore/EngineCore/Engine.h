#pragma once

#include <vector>
#include <chrono>
#include <entt.hpp>

#include "Core/SystemsState.h"
#include "EngineCore/Defines/DllDefines.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Managers/InputManager.h"
#include "EngineCore/Core/ProjectSettings.h"

#include "EngineCore/Rollback/Rollback.h"
#include "Serialization/EntitySerializer.h"

#if GLFW_INCLUDE_VULKAN
#include "EngineCore/Core/AssetsDatabaseVulkan.h"
#include "EngineCore/Core/AssetsLoaderVulkan.h"
#include "EngineCore/Rendering/Vulkan/GraphicsApiVulkan.h"
#endif

namespace EngineCore
{
	class Engine
	{
	public:
		Engine(const Ref<ProjectSettings>& projectSettings);
		virtual ~Engine();
	
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

#if GLFW_INCLUDE_VULKAN
		AVulkan::GraphicsApiVulkan* GetGraphicsApi() const { return graphicsApi; }
		Ref<AssetsDatabaseVulkan> GetAssetsDatabase() { return assetsDatabase; }
		Ref<AssetsLoaderVulkan> GetResourcesManager() { return resourcesManager; }
#else
		IGraphicsApi* GetGraphicsApi() const { return graphicsApi; }
		Ref<AssetsDatabase> GetAssetsDatabase() { return assetsDatabase; }
		Ref<AssetsLoader> GetResourcesManager() { return resourcesManager; }
#endif

		GLFWwindow* GetWindow() const { return window; }
		Ref<Ecs> GetEcs() { return ecs; }
		Ref<InputManager> GetInput() { return input; }
		Ref<EngineCore::EntitySerializer> GetSerializer() { return entitySerializer; }
		Ref<SystemsState> GetSystemsState() { return systemsState; }
	
		float GetDeltaTime() const { return deltaTime; }

	private:
		const Ref<ProjectSettings>& projectSettings;

		GLFWwindow* window;

#if GLFW_INCLUDE_VULKAN
		AVulkan::GraphicsApiVulkan* graphicsApi;
		Ref<AssetsDatabaseVulkan> assetsDatabase;
		Ref<AssetsLoaderVulkan> resourcesManager;
#else
		IGraphicsApi* graphicsApi;
		Ref<AssetsDatabase> assetsDatabase;
		Ref<AssetsLoader> resourcesManager;
#endif
	
		Ref<Ecs> ecs;
		Ref<InputManager> input;
		Ref<SystemsState> systemsState;
		Ref<EngineCore::EntitySerializer> entitySerializer;
	
		float deltaTime;

		std::function<void()> gameSystemsUpdate;
		std::function<void()> editorUpdate;

		std::chrono::steady_clock::time_point cachedTime;

	};
}