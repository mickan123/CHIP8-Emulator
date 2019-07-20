#pragma once

#include <string>
#include <GLFW\glfw3.h>


const unsigned char chip8Fontset[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

const int glfwKeys[16] = { GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4,
						   GLFW_KEY_Q, GLFW_KEY_W, GLFW_KEY_E, GLFW_KEY_R,
						   GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_F,
						   GLFW_KEY_Z, GLFW_KEY_X, GLFW_KEY_C, GLFW_KEY_V };

class chip8 {
public:
	chip8();

	// Initialise cpu state
	void initialise();

	// Loads a game to play
	void loadGame(std::string game);

	// Runs one cycle 
	void cycle();

	// Renders the window
	void render();

	// Store key press state in emulator
	void setKeys(GLFWwindow* window);

	bool drawFlag = false;

private:
	// Graphics constants
	static const int chip8_width = 64;
	static const int chip8_height = 32;
	static const int scale = 10;

	// Memory constants
	static const int memorySize = 4096;
	static const int memoryOffset = 512;

	// 0x0000 to 0x01FF Chip 8 Interpreter
	// 0x0200 to 0x0E9F Program RAM
	// 0x0EA0 to 0x0EFF Call stack, internal use and other variables
	// 0x0F00 to 0x0FFF Display Refresh
	unsigned char memory[memorySize];

	// 16 Registers V0 to VE, 16th register is used for carry flag
	unsigned char registers[16];

	// Index register and program counter registers
	unsigned short I;
	unsigned short pc;

	// 64 x 32 graphics display black or white
	unsigned char graphics[chip8_width][chip8_height];

	// Two timers that count at 60Hz, when greater than 0
	// they will count down to 0
	unsigned char delayTimer;
	unsigned char soundTimer;

	// Stack with 16 levels to support subroutines
	unsigned short stack[16];
	unsigned short sp;

	// Hex based keyboard 0x0 to 0xF
	unsigned char key[16];

	// 35 possible opcodes
	unsigned short opcode;

	// Instruction decode/execute functions
	void zeroInstruction();
	void eightInstruction();
	void eInstruction();
	void fInstruction();
	void invalidInstruction();

	void clearDisplay();
};