#include "application.h"

#include <iostream>

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

	// glad: load all OpenGL function pointers
	/*
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}*/
}


void application::start() {
	glMatrixMode(GL_PROJECTION);

	
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 100, -100);
		glMatrixMode(GL_MODELVIEW);

		emulator.render();

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
