#pragma once

#include <GLFW/glfw3.h>
#include "EngineCore/Defines/DllDefines.h"

class Input
{
public: 
	Input(GLFWwindow* window);
	~Input();

	void Update();

	GLFWkeyfun SetKeyCallback(GLFWkeyfun callback);
	GLFWkeyfun SetKeyCallback(GLFWwindow* window, GLFWkeyfun callback);

	GLFWcursorposfun SetCursorPosCallback(GLFWcursorposfun callback);
	GLFWcursorposfun SetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun callback);

	GLFWmousebuttonfun SetMouseButtonCallback(GLFWmousebuttonfun callback);
	GLFWmousebuttonfun SetMouseButtonCallback(GLFWwindow* window, GLFWmousebuttonfun callback);

	bool IsKeyPressed(int key);
	bool IsKeyPressed(GLFWwindow* window, int key);

	bool IsKeyReleased(int key);
	bool IsKeyReleased(GLFWwindow* window, int key);

	bool IsKeyHold(int key);
	bool IsKeyHold(GLFWwindow* window, int key);

	bool IsMousePressed(int button);
	bool IsMousePressed(GLFWwindow* window, int button);

	bool IsMouseReleased(int button);
	bool IsMouseReleased(GLFWwindow* window, int button);

	void GetCursorPosition(double& x, double& y);
	void GetCursorPosition(GLFWwindow* window, double& x, double& y);

	void GetCursorDelta(double& x, double& y);

private:
	GLFWwindow* cachedWindow;

	double cursorDeltaX;
	double cursorDeltaY;

	double prevCursorPosX;
	double prevCursorPosY;

	void UpdateCursorDelta();
	void ResetCursorDelta();
};

