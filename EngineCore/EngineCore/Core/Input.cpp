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

GLFWkeyfun Input::SetKeyCallback(const GLFWkeyfun callback) const
{
	return SetKeyCallback(cachedWindow, callback);
}

GLFWkeyfun Input::SetKeyCallback(GLFWwindow* window, const GLFWkeyfun callback) const
{
	return glfwSetKeyCallback(window, callback);
}

GLFWcursorposfun Input::SetCursorPosCallback(const GLFWcursorposfun callback) const
{
	return SetCursorPosCallback(cachedWindow, callback);
}

GLFWcursorposfun Input::SetCursorPosCallback(GLFWwindow* window, const GLFWcursorposfun callback) const
{
	return glfwSetCursorPosCallback(window, callback);
}

GLFWmousebuttonfun Input::SetMouseButtonCallback(const GLFWmousebuttonfun callback) const
{
	return SetMouseButtonCallback(cachedWindow, callback);
}

GLFWmousebuttonfun Input::SetMouseButtonCallback(GLFWwindow* window, const GLFWmousebuttonfun callback) const
{
	return glfwSetMouseButtonCallback(window, callback);
}

bool Input::IsKeyPressed(const int key) const
{
	return IsKeyPressed(cachedWindow, key);
}

bool Input::IsKeyPressed(GLFWwindow* window, const int key) const
{
	auto state = glfwGetKey(window, key);
	return state == GLFW_PRESS;
}

bool Input::IsKeyReleased(const int key) const
{
	return IsKeyReleased(cachedWindow, key);
}

bool Input::IsKeyReleased(GLFWwindow* window, const int key) const
{
	auto state = glfwGetKey(window, key);
	return state == GLFW_RELEASE;
}

//what exactly the function for?
bool Input::IsKeyHold(const int key) const
{
	return IsKeyHold(cachedWindow, key);
}

//what exactly the function for?
bool Input::IsKeyHold(GLFWwindow* window, const int key) const
{
	auto state = glfwGetKey(window, key);
	return state == GLFW_REPEAT;
}

bool Input::IsMousePressed(const int button) const
{
	return IsMousePressed(cachedWindow, button);
}

bool Input::IsMouseReleased(const int button) const
{
	return IsMouseReleased(cachedWindow, button);
}

bool Input::IsMousePressed(GLFWwindow* window, const int button) const
{
	return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

bool Input::IsMouseReleased(GLFWwindow* window, const int button) const
{
	return glfwGetMouseButton(window, button) == GLFW_RELEASE;
}

void Input::GetCursorPosition(double& x, double& y) const
{
	GetCursorPosition(cachedWindow, x, y);
}

void Input::GetCursorPosition(GLFWwindow* window, double& x, double& y) const
{
	glfwGetCursorPos(window, &x, &y);
}

void Input::GetCursorDelta(double& x, double& y) const
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
