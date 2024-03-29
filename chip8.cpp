#include "chip8.h"

#include <GLFW\glfw3.h>
#include <iostream>
#include <time.h>


chip8::chip8() {
	initialise();
}

// Resets all emulator components
void chip8::initialise() {
	// Initialise registers and memory
	pc = 0x200;
	opcode = 0;
	I = 0;
	sp = 0;

	clearDisplay();

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

	// Set seed
	srand(time(NULL));

	// Load binary
	loadGame("C:/Users/user/Documents/Projects/CHIP8-Emulator/roms/Breakout (Brix hack) [David Winter, 1997].ch8");
}

// Clears display by zeroing all values
void chip8::clearDisplay() {
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 32; j++) {
			graphics[i][j] = 0;
		}
	}
	drawFlag = true;
}

// Reads binary rom from input string file path
void chip8::loadGame(std::string game) {
	FILE *source = fopen(game.c_str(), "rb");

	const int bufferSize = memorySize - memoryOffset;
	char buffer[bufferSize];
	size_t n;
	if (source)
	{
		n = fread(buffer, sizeof(unsigned char), bufferSize, source);
	}
	else
	{
		std::cout << "Error opening file" << std::endl;
	}
	for (int i = 0; i < n; i++) {
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
				
	glColor3f(1, 1, 1); // Set colour to black

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
unsigned char prev = 0;

// Executes one emulation cycle, reference: https://en.wikipedia.org/wiki/CHIP-8#Opcode_table
void chip8::cycle() {
	// Fetch Opcode
	opcode = memory[pc] << 8 | memory[pc + 1];
	unsigned short X = (opcode & 0x0F00) >> 8;
	unsigned short Y = (opcode & 0x00F0) >> 4;
	unsigned short F = 0xF;

	// Decode/Execute Opcode
	switch (opcode & 0xF000) {
	case(0x0000):
		zeroInstruction();
		break;
	case(0x1000):
		pc = (opcode & 0x0FFF);
		break;
	case(0x2000):
		stack[sp] = pc;
		sp++;
		pc = (opcode & 0x0FFF);
		break;
	case(0x3000):
		if (registers[X] == (opcode & 0x00FF)) {
			pc += 4;
		}
		else {
			pc += 2;
		}
		break;
	case(0x4000):
		if (registers[X] != (opcode & 0x00FF)) {
			pc += 4;
		}
		else {
			pc += 2;
		}
		break;
	case(0x5000):
		if (registers[X] == registers[Y]) {
			pc += 4;
		}
		else {
			pc += 2;
		}
		break;
	case(0x6000):
		registers[X] = (opcode & 0x00FF);
		pc += 2;
		break;
	case(0x7000):
		registers[X] += (opcode & 0x00FF);
		pc += 2;
		break;
	case(0x8000):
		eightInstruction();
		break;
	case(0x9000):
		if (registers[X] != registers[Y]) {
			pc += 4;
		}
		else {
			pc += 2;
		}
		break;
	case(0xA000):
		I = (opcode & 0x0FFF);
		pc += 2;
		break;
	case(0xB000):
		pc = (opcode & 0x0FFF) + registers[0];
		break;
	case(0xC000):
		registers[X] = (opcode & 0x00FF) & (rand() % 256);
		pc += 2;
		break;
	case(0xD000):
	{
		int height = opcode & 0x000F;
		unsigned short pixel;

		registers[F] = 0;

		for (int y = 0; y < height; y++) {
			pixel = memory[I + y];

			for (int x = 0; x < 8; x++) {

				if ((pixel & (0x80 >> x)) != 0) {
					
					int graphX = (x + registers[X]) % chip8_width;
					int graphY = (y + registers[Y]) % chip8_height;
					if (graphics[graphX][graphY] == 1) {
						registers[F] = 1;
					}
					graphics[graphX][graphY] ^= 1;
				}
			}
		}

		pc += 2;
		drawFlag = true;
		break;
	}
	case(0xE000):
		eInstruction();
		break;
	case(0xF000):
		fInstruction();
		break;
	default:
		break;
	}

	// Update timers
	if (delayTimer > 0) {
		delayTimer--;
	}
	if (soundTimer == 1) {
		std::cout << "Beep!" << std::endl;
		soundTimer--;
	}
}

// Decodes/exectutes instructions that have a 0x0 most significant byte
void chip8::zeroInstruction() {
	switch (opcode & 0x00FF) {
	case(0x00E0):
		clearDisplay();
		break;
	case(0x00EE):
		sp--;
		pc = stack[sp];
		break;
	default:
		invalidInstruction();
		break;
	}
	pc += 2;
}

// Decodes/exectutes instructions that have an 0x8 most significant byte
void chip8::eightInstruction() {
	unsigned short X = (opcode & 0x0F00) >> 8;
	unsigned short Y = (opcode & 0x00F0) >> 4;
	unsigned short F = 0xF;
	
	switch (opcode & 0x000F) {
	case(0x0000):
		registers[X] = registers[Y];
		pc += 2;
		break;
	case(0x0001):
		registers[X] |= registers[X];
		pc += 2;
		break;
	case(0x0002):
		registers[X] &= registers[Y];
		pc += 2;
		break;
	case(0x0003):
		registers[X] ^= registers[Y];
		pc += 2;
		break;
	case(0x0004):
		if ((int)registers[X] + (int)registers[Y] > 255) {
			registers[F] = 1;
		}
		else {
			registers[F] = 0;
		}
		registers[X] += registers[Y];
		pc += 2;
		break;
	case(0x0005):
		if (registers[X] >= registers[Y]) {
			registers[F] = 1;
		}
		else {
			registers[F] = 0;
		}
		registers[X] -= registers[Y];
		pc += 2;
		break;
	case(0x0006):
		registers[F] = registers[X] & 0x1;
		registers[X] >>= 1;
		pc += 2;
		break;
	case(0x0007):
		if (registers[Y] >= registers[X]) {
			registers[F] = 1;
		}
		else {
			registers[F] = 0;
		}
		registers[X] = registers[Y] - registers[X];
		pc += 2;
		break;
	case(0x000E):
		registers[F] = (registers[X] & 0x80) >> 7;
		registers[X] <<= 1;
		pc += 2;
		break;
	default:
		invalidInstruction();
		break;
	}
	
}

// Decodes/exectutes instructions that have an 0xE most significant byte
void chip8::eInstruction() {
	unsigned short X = (opcode & 0x0F00) >> 8;

	switch (opcode & 0x00FF) {
	case(0x009E):
		if (key[registers[X]] != 0) {
			pc += 4;
		}
		else {
			pc += 2;
		}
		break;
	case(0x00A1):
		if (key[registers[X]] == 0) {
			pc += 4;
		}
		else {
			pc += 2;
		}
		break;
	default:
		invalidInstruction();
		break;
	}
}
// Decodes/exectutes instructions that have an 0xF most significant byte
void chip8::fInstruction() {
	unsigned short X = (opcode & 0x0F00) >> 8;
	unsigned short F = 0xF;
	

	switch (opcode & 0x00FF) {
	case(0x0007):
		registers[X] = delayTimer;
		pc += 2;
		break;
	case(0x000A):
	{
		bool keyPress = false;
		
		for (int i = 0; i < 16; i++) {
			if (key[i] != 0) {
				registers[X] = i;
				keyPress = true;
				break;
			}
		}
		if (!keyPress) {
			return;
		}
		pc += 2;
		break;
	}
	case(0x0015):
		delayTimer = registers[X];
		pc += 2;
		break;
	case(0x0018):
		soundTimer = registers[X];
		pc += 2;
		break;
	case(0x001E):
		if ((I + registers[X]) > 0xFFF) {
			registers[F] = 1;
		}
		else {
			registers[F] = 0;
		}
		I += registers[X];
		pc += 2;
		break;
	case(0x0029):
		I = registers[X] * 0x5;
		pc += 2;
		break;
	case(0x0033):
		memory[I] =      registers[X] / 100;
		memory[I + 1] = (registers[X] % 100) / 10;
		memory[I + 2] = (registers[X] % 10);
		pc += 2;
		break;
	case(0x0055):
		for (int i = 0; i <= X; i++) {
			memory[I + i] = registers[i];
		}
		I += X + 1;
		pc += 2;
		break;
	case(0x0065):
		for (int i = 0; i <= X; i++) {
			registers[i] = memory[I + i];
		}
		I += X + 1;
		pc += 2;
		break;
	default:
		invalidInstruction();
		break;
	}
}

void chip8::invalidInstruction() {
	std::cout << "Invalid instruction: " << std::hex << opcode << std::endl;
}


