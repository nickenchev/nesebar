#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "common.hpp"

namespace mos6502
{

struct Instruction
{
	const byte opcode;
	const char *name;
	const short byteSize;
	const short cycles;
	const byte flagsAffected;

	constexpr Instruction(const byte opcode, const char *name,
						  short byteSize, short cycles, byte flagsAffected)
		: opcode(opcode), name(name), byteSize(byteSize),
		  cycles(cycles), flagsAffected(flagsAffected)
	{
	}

	constexpr operator byte() const
	{
		return opcode;
	}
};

};

#endif /* INSTRUCTION_H */
