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
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, -1);

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;	
	
	while (!glfwWindowShouldClose(window)) {

		// Get time since last frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// If running faster than refresh rate then sleep to make up difference
		if (deltaTime < refreshRate)
		{
			int sleepTime = (refreshRate - deltaTime) * 1000;
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
		}

		// Emulate one cycle
		emulator.cycle();

		// Draw if flag is set
		if (emulator.drawFlag) {
			display();
		}

		// Update keys pressed
		emulator.setKeys(window);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}

void application::display() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	emulator.render();
	emulator.drawFlag = false;
}

void application::keyCallback(GLFWwindow* window, int key, int scandcode, int action, int mode) {
	std::cout << "Callback" << std::endl;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		std::cout << "HERE" << std::endl;
		glfwSetWindowShouldClose(window, true);
	}
}


void application::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
