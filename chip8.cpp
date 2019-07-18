#include "chip8.h"

#include <GLFW\glfw3.h>
#include <iostream>


chip8::chip8() {
	for (int i = 0; i < chip8_width; i++) {
		for (int j = 0; j < chip8_height; j++) {
			if (i == 0 || j == 0 || i == chip8_width - 1 || j == chip8_height - 1) {
				graphics[i][j] = 1;
			}
			else {
				graphics[i][j] = 0;
			}
		}
	}
}


void chip8::initialise() {
	// Initialise registers and memory
	pc = 0x200;
	opcode = 0;
	I = 0;
	sp = 0;

	// Clear display
	// Clear stack
	// Clear registers V0-VF
	// Clear memory

	// Load fontset
	for (int i = 0; i < 80; ++i)
		memory[i] = chip8Fontset[i];

	// Reset timers
}

void chip8::loadGame(std::string game) {

	// fopen in binary mode 
	/*
	for (int i = 0; i < bufferSize; ++i)
		memory[i + 512] = buffer[i];*/
}

void chip8::cycle() {
	// Fetch Opcode
	// Decode Opcode
	// Execute Opcode

	// Update timers
}

void chip8::setKeys(GLFWwindow* window) {
	for (int i = 0; i < 16; i++) {
		key[i] = glfwGetKey(window, glfwKeys[i]);
	}
}

void chip8::render() {
	glPointSize(scale);
	int offset = scale / 2; // Add offset because we want to index top left of point
	glColor3f(1, 1, 1);
	for (int i = 0; i < chip8_width; i++) {
		for (int j = 0; j < chip8_height; j++) {
			if (graphics[i][j] == 1) {
				glBegin(GL_POINTS);
				glVertex2i(i*scale + offset, j*scale + offset);
				glEnd();
			}
		}
	}
}