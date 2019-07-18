#pragma once
#include "chip8.h"

#include <GLFW/glfw3.h>

const GLuint SCREEN_WIDTH = 640;
const GLuint SCREEN_HEIGHT = 320;

class application
{
public:
	application();
	~application();

	void start();
	void setup();

private:
	chip8 emulator;
	GLFWwindow* window;

	static void keyCallback(GLFWwindow* window, int key, int scandcode, int action, int mode);
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	const GLfloat refreshRate = 1.0f / 60.0f; // 60 Hz refresh rate
};