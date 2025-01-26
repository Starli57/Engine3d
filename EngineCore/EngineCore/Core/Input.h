#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Defines/DllDefines.h"

class Input
{
public: 
	Input(GLFWwindow* window);
	~Input();

	void Update();

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

	bool IsMousePressed(int button) const;
	bool IsMousePressed(GLFWwindow* window, int button) const;

	bool IsMouseReleased(int button) const;
	bool IsMouseReleased(GLFWwindow* window, int button) const;

	void GetCursorPosition(double& x, double& y) const;
	void GetCursorPosition(GLFWwindow* window, double& x, double& y) const;

	void GetCursorDelta(double& x, double& y) const;

private:
	GLFWwindow* cachedWindow;

	double cursorDeltaX;
	double cursorDeltaY;

	double prevCursorPosX;
	double prevCursorPosY;

	void UpdateCursorDelta();
	void ResetCursorDelta();
};

