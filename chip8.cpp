#include "chip8.h"

#include <GLFW\glfw3.h>
#include <iostream>


chip8::chip8() {
	initialise();
}


void chip8::initialise() {
	// Initialise registers and memory
	pc = 0x200;
	opcode = 0;
	I = 0;
	sp = 0;

	// Clear display
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 32; j++) {
			graphics[i][j] = 0;
		}
	}

	// Clear stack
	for (int i = 0; i < 16; i++) {
		stack[i] = 0;
	}

	// Clear registers V0-VF
	for (int i = 0; i < 16; i++) {
		registers[i] = 0;
	}

	// Clear memory
	for (int i = 0; i < memorySize; i++) {
		memory[i] = 0;
	}

	// Load fontset
	for (int i = 0; i < 80; ++i)
		memory[i] = chip8Fontset[i];

	// Reset timers
	delayTimer = 0;
	soundTimer = 0;

	// Load binary
	loadGame("C:/Users/user/Documents/Projects/CHIP8-Emulator/roms/Brix [Andreas Gustafsson, 1990].ch8");
}

// Reads binary rom from input string file path
void chip8::loadGame(std::string game) {
	FILE *source = fopen(game.c_str(), "rb");

	const int bufferSize = memorySize - memoryOffset;
	char buffer[bufferSize];
	if (source)
	{
		fread(buffer, bufferSize, 1, source);
	}
	else
	{
		std::cout << "Error opening file" << std::endl;
	}
	for (int i = 0; i < bufferSize; i++) {
		memory[i + memoryOffset] = buffer[i];
	}
}

// Use GLFW functions get key states
void chip8::setKeys(GLFWwindow* window) {
	for (int i = 0; i < 16; i++) {
		key[i] = glfwGetKey(window, glfwKeys[i]);
	}
}

// Renders each pixel 
void chip8::render() {
	glPointSize(scale); // Draw a larger point because we don't want a 64x32 screen
	int offset = scale / 2; // Add offset because we want to index top left of point

							// Set colour to black
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

// Executes one emulation cycle
void chip8::cycle() {
	// Fetch Opcode
	// Decode Opcode
	// Execute Opcode

	// Update timers
}



