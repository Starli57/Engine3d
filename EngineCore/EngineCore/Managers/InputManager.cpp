#include "EngineCore/Pch.h"
#include "InputManager.h"
#include "EngineCore/Components/PositionComponent.h"

namespace EngineCore
{
	InputManager::InputManager(GLFWwindow* window, const Ref<Ecs>& ecs)
	{
		this->cachedWindow = window;
		this->ecs = ecs;
	
		UpdateCursorPosition();
		ResetCursorDelta();
	}

	void InputManager::Update()
	{
		UpdateCursorPosition();
		UpdateLeftMouseButtonState();
	}

	void InputManager::UpdateLeftMouseButtonState()
	{
		//todo: make the logic without boolean flags
		if (IsMousePressedStay(0))
		{
			onLeftMouseButtonDown = !wasLeftMouseButtonPressed;
			wasLeftMouseButtonPressed = true;
		}
		else
		{
			onLeftMouseButtonDown = false;
			wasLeftMouseButtonPressed = false;
		}
	}

	GLFWkeyfun InputManager::SetKeyCallback(const GLFWkeyfun callback) const
	{
		return SetKeyCallback(cachedWindow, callback);
	}

	GLFWkeyfun InputManager::SetKeyCallback(GLFWwindow* window, const GLFWkeyfun callback) const
	{
		return glfwSetKeyCallback(window, callback);
	}

	GLFWcursorposfun InputManager::SetCursorPosCallback(const GLFWcursorposfun callback) const
	{
		return SetCursorPosCallback(cachedWindow, callback);
	}

	GLFWcursorposfun InputManager::SetCursorPosCallback(GLFWwindow* window, const GLFWcursorposfun callback) const
	{
		return glfwSetCursorPosCallback(window, callback);
	}

	GLFWmousebuttonfun InputManager::SetMouseButtonCallback(const GLFWmousebuttonfun callback) const
	{
		return SetMouseButtonCallback(cachedWindow, callback);
	}

	GLFWmousebuttonfun InputManager::SetMouseButtonCallback(GLFWwindow* window, const GLFWmousebuttonfun callback) const
	{
		return glfwSetMouseButtonCallback(window, callback);
	}

	bool InputManager::IsKeyPressed(const int key) const
	{
		return IsKeyPressed(cachedWindow, key);
	}

	bool InputManager::IsKeyPressed(GLFWwindow* window, const int key) const
	{
		auto state = glfwGetKey(window, key);
		return state == GLFW_PRESS;
	}

	bool InputManager::IsKeyReleased(const int key) const
	{
		return IsKeyReleased(cachedWindow, key);
	}

	bool InputManager::IsKeyReleased(GLFWwindow* window, const int key) const
	{
		auto state = glfwGetKey(window, key);
		return state == GLFW_RELEASE;
	}

	//what exactly the function for?
	bool InputManager::IsKeyHold(const int key) const
	{
		return IsKeyHold(cachedWindow, key);
	}

	//what exactly the function for?
	bool InputManager::IsKeyHold(GLFWwindow* window, const int key) const
	{
		auto state = glfwGetKey(window, key);
		return state == GLFW_REPEAT;
	}

	bool InputManager::IsMousePressedStay(const int button) const
	{
		return IsMousePressedStay(cachedWindow, button);
	}

	bool InputManager::IsMouseReleased(const int button) const
	{
		return IsMouseReleased(cachedWindow, button);
	}

	bool InputManager::IsMousePressedStay(GLFWwindow* window, const int button) const
	{
		return glfwGetMouseButton(window, button) == GLFW_PRESS;
	}

	bool InputManager::IsMouseReleased(GLFWwindow* window, const int button) const
	{
		return glfwGetMouseButton(window, button) == GLFW_RELEASE;
	}

	bool InputManager::IsLeftMousePressed() const
	{
		return onLeftMouseButtonDown;
	}

	void InputManager::GetCursorPosition(double& x, double& y) const
	{
		GetCursorPosition(cachedWindow, x, y);
	}

	void InputManager::GetCursorPosition(GLFWwindow* window, double& x, double& y) const
	{
		glfwGetCursorPos(window, &x, &y);
	}

	void InputManager::GetCursorDelta(double& x, double& y) const
	{
		x = cursorDeltaX;
		y = cursorDeltaY;
	}

	void InputManager::GetCursorWorldPosition(glm::vec3& position) const
	{
		position = cursorWorldPosition;
	}

	void InputManager::GetCursorPlacementWorldPosition(glm::vec3& position) const
	{
		position = cursorPlacementWorldPosition;
	}

	void InputManager::GetCameraLookAtPosition(glm::vec3& position) const
	{
		position = cameraLookAtPosition;
	}

	void InputManager::UpdateCursorPosition()
	{
		double newX;
		double newY;
		GetCursorPosition(newX, newY);

		cursorDeltaX = newX - cursorPositionX;
		cursorDeltaY = newY - cursorPositionY;

		cursorPositionX = newX;
		cursorPositionY = newY;
	}

	void InputManager::ResetCursorDelta()
	{
		cursorDeltaX = 0;
		cursorDeltaY = 0;
	}
}