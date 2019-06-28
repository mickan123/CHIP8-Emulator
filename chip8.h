#pragma once

#include <string>


class chip8 {
public:
	chip8();

	// Loads a game to play
	void loadGame(std::string game);

	// Runs one cycle 
	void cycle();

private:
	// 0x0000 to 0x01FF Chip 8 Interpreter
	// 0x0200 to 0x0E9F Program RAM
	// 0x0EA0 to 0x0EFF Call stack, internal use and other variables
	// 0x0F00 to 0x0FFF Display Refresh
	unsigned char memory[4096];

	// 16 Registers V0 to VE, 16th register is used for carry flag
	unsigned char registers[16];

	// Index register and program counter registers
	unsigned short I;
	unsigned short pc;

	// 64 x 32 graphics display black or white
	unsigned char graphics[64][32];

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
};