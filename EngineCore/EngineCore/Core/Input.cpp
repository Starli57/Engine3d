#include "EngineCore/Pch.h"
#include "Input.h"

Input::Input(GLFWwindow* window)
{
	this->cachedWindow = window;
}

Input::~Input()
{
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

void Input::GetCursorPosition(double& x, double& y)
{
	GetCursorPosition(cachedWindow, x, y);
}

void Input::GetCursorPosition(GLFWwindow* window, double& x, double& y)
{
	glfwGetCursorPos(window, &x, &y);
}
