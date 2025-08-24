#pragma once

#include <GLFW/glfw3.h>

#include "EngineCore/Core/Ecs.h"
#include "glm/vec3.hpp"

namespace Engine
{
	class InputManager
	{
	public: 
		InputManager(GLFWwindow* window, const Ref<Ecs>& ecs);

		void Update();
		void UpdateLeftMouseButtonState();
	
		GLFWkeyfun SetKeyCallback(GLFWkeyfun callback) const;
		GLFWkeyfun SetKeyCallback(GLFWwindow* window, GLFWkeyfun callback) const;

		GLFWcursorposfun SetCursorPosCallback(GLFWcursorposfun callback) const;
		GLFWcursorposfun SetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun callback) const;

		GLFWmousebuttonfun SetMouseButtonCallback(GLFWmousebuttonfun callback) const;
		GLFWmousebuttonfun SetMouseButtonCallback(GLFWwindow* window, GLFWmousebuttonfun callback) const;

		bool IsKeyPressed(int key) const;
		bool IsKeyPressed(GLFWwindow* window, int key) const;

		bool IsKeyReleased(int key) const;
		bool IsKeyReleased(GLFWwindow* window, int key) const;

		bool IsKeyHold(int key) const;
		bool IsKeyHold(GLFWwindow* window, int key) const;

		bool IsMousePressedStay(int button) const;
		bool IsMousePressedStay(GLFWwindow* window, int button) const;

		bool IsMouseReleased(int button) const;
		bool IsMouseReleased(GLFWwindow* window, int button) const;

		bool IsLeftMousePressed() const;
	
		void GetCursorPosition(double& x, double& y) const;
		void GetCursorPosition(GLFWwindow* window, double& x, double& y) const;
	
		void GetCursorDelta(double& x, double& y) const;

		void GetCursorWorldPosition(glm::vec3& position) const;
		void GetCursorPlacementWorldPosition(glm::vec3& position) const;
		void GetCameraLookAtPosition(glm::vec3& position) const;

		void SetSelectableLookAtEntity(const entt::entity entity) { selectableLookAtEntity = entity; }

		glm::vec3 cursorWorldPosition;
		glm::vec3 cursorPlacementWorldPosition;
		glm::vec3 cameraLookAtPosition;
		entt::entity& GetSelectableLookAtEntity() { return selectableLookAtEntity; }
	
	private:
		GLFWwindow* cachedWindow;
		Ref<Ecs> ecs;
	
		double cursorDeltaX;
		double cursorDeltaY;

		double cursorPositionX;
		double cursorPositionY;
	
		void UpdateCursorPosition();
		void ResetCursorDelta();

		entt::entity selectableLookAtEntity;

		bool wasLeftMouseButtonPressed;
		bool onLeftMouseButtonDown;
	};
}