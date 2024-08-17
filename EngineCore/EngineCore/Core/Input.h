#pragma once

#include <GLFW/glfw3.h>

class Input
{
public: 
	Input(GLFWwindow* window);
	~Input();

	GLFWkeyfun SetKeyCallback(GLFWkeyfun callback);
	GLFWkeyfun SetKeyCallback(GLFWwindow* window, GLFWkeyfun callback);

	GLFWcursorposfun SetCursorPosCallback(GLFWcursorposfun callback);
	GLFWcursorposfun SetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun callback);

	bool IsKeyPressed(int key);
	bool IsKeyPressed(GLFWwindow* window, int key);

	bool IsKeyReleased(int key);
	bool IsKeyReleased(GLFWwindow* window, int key);

	bool IsKeyHold(int key);
	bool IsKeyHold(GLFWwindow* window, int key);

	void GetCursorPosition(double& x, double& y);
	void GetCursorPosition(GLFWwindow* window, double& x, double& y);

private:
	GLFWwindow* cachedWindow;
};

