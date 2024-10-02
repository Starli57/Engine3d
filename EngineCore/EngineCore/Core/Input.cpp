#include "EngineCore/Pch.h"
#include "Input.h"

Input::Input(GLFWwindow* window)
{
	this->cachedWindow = window;
	UpdateCursorDelta();
	ResetCursorDelta();
}

Input::~Input()
{
}

void Input::Update()
{
	UpdateCursorDelta();
}

GLFWkeyfun Input::SetKeyCallback(GLFWkeyfun callback)
{
	return SetKeyCallback(cachedWindow, callback);
}

GLFWkeyfun Input::SetKeyCallback(GLFWwindow* window, GLFWkeyfun callback)
{
	return glfwSetKeyCallback(window, callback);
}

GLFWcursorposfun Input::SetCursorPosCallback(GLFWcursorposfun callback)
{
	return SetCursorPosCallback(cachedWindow, callback);
}

GLFWcursorposfun Input::SetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun callback)
{
	return glfwSetCursorPosCallback(window, callback);
}

GLFWmousebuttonfun Input::SetMouseButtonCallback(GLFWmousebuttonfun callback)
{
	return SetMouseButtonCallback(cachedWindow, callback);
}

GLFWmousebuttonfun Input::SetMouseButtonCallback(GLFWwindow* window, GLFWmousebuttonfun callback)
{
	return glfwSetMouseButtonCallback(window, callback);
}

bool Input::IsKeyPressed(int key)
{
	return IsKeyPressed(cachedWindow, key);
}

bool Input::IsKeyPressed(GLFWwindow* window, int key)
{
	auto state = glfwGetKey(window, key);
	return state == GLFW_PRESS;
}

bool Input::IsKeyReleased(int key)
{
	return IsKeyReleased(cachedWindow, key);
}

bool Input::IsKeyReleased(GLFWwindow* window, int key)
{
	auto state = glfwGetKey(window, key);
	return state == GLFW_RELEASE;
}

//what exactly the function for?
bool Input::IsKeyHold(int key)
{
	return IsKeyHold(cachedWindow, key);
}

//what exactly the function for?
bool Input::IsKeyHold(GLFWwindow* window, int key)
{
	auto state = glfwGetKey(window, key);
	return state == GLFW_REPEAT;
}

bool Input::IsMousePressed(int button)
{
	return IsMousePressed(cachedWindow, button);
}

bool Input::IsMouseReleased(int button)
{
	return IsMouseReleased(cachedWindow, button);
}

bool Input::IsMousePressed(GLFWwindow* window, int button)
{
	return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

bool Input::IsMouseReleased(GLFWwindow* window, int button)
{
	return glfwGetMouseButton(window, button) == GLFW_RELEASE;
}

void Input::GetCursorPosition(double& x, double& y)
{
	GetCursorPosition(cachedWindow, x, y);
}

void Input::GetCursorPosition(GLFWwindow* window, double& x, double& y)
{
	glfwGetCursorPos(window, &x, &y);
}

void Input::GetCursorDelta(double& x, double& y)
{
	x = cursorDeltaX;
	y = cursorDeltaY;
}

void Input::UpdateCursorDelta()
{
	double newX;
	double newY;
	GetCursorPosition(newX, newY);

	cursorDeltaX = newX - prevCursorPosX;
	cursorDeltaY = newY - prevCursorPosY;

	prevCursorPosX = newX;
	prevCursorPosY = newY;
}

void Input::ResetCursorDelta()
{
	cursorDeltaX = 0;
	cursorDeltaY = 0;
}
