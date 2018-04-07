#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "common.hpp"

class Instruction
{
	std::string name;
	short pcStep;
	short cycles;

public:
	Instruction() : name("")
	{
		pcStep = 0;
		cycles = 0;
	}

	void begin(const std::string &name, short pcStep, short cycles)
	{
		this->name = name;
		this->pcStep = pcStep;
		this->cycles = cycles;
	}
	void end()
	{
		name = "Unknown";
		pcStep = 0;
		cycles = 0;
	}

	void increaseCycles(short count) { cycles += count; }
	const std::string &getName() const { return name; }
	const short &getStepSize() const { return pcStep; }
	const short &getCycles() const { return cycles; }
};

#endif /* INSTRUCTION_H */
