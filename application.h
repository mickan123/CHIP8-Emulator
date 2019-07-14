#pragma once
#include "chip8.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;

class application
{
public:
	application();
	~application();

	void start();
	void setup();

private:
	chip8 cpu;
	GLFWwindow* window;

	static void keyCallback(GLFWwindow* window, int key, int scandcode, int action, int mode);
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};