#include "application.h"

#include <iostream>
#include <thread>
#include <chrono>

application::application() {
	setup();
}

application::~application() {
	glfwTerminate();
}

void application::setup() {
	glfwInit();

	// glfw window creation
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chip8-Emulator", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Capture mouse movements
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(window, this);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, this->keyCallback);
	glfwSetFramebufferSizeCallback(window, this->framebufferSizeCallback);
}


void application::start() {
	glMatrixMode(GL_PROJECTION);

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	GLfloat refreshRate = 1.0f / 60.0f; // 60 Hz refresh rate

	while (!glfwWindowShouldClose(window)) {

		GLfloat currentFrame = glfwGetTime();
		std::cout << currentFrame << std::endl;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (deltaTime < refreshRate)
		{
			int sleepTime = (refreshRate - deltaTime) * 1000;
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
		}


		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 100, -100);
		glMatrixMode(GL_MODELVIEW);

		// Emulate one cycle
		emulator.cycle();

		// Draw if flag is set
		if (emulator.drawFlag) {
			emulator.render();
		}

		emulator.setKeys(window);

		glfwSwapBuffers(window);
	}
}

void application::keyCallback(GLFWwindow* window, int key, int scandcode, int action, int mode) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}


void application::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
