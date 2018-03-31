#ifndef CORE6502_H
#define CORE6502_H

#include <bitset>
#include <cstdint>
#include <vector>
#include "common.hpp"
#include "mem_address.hpp"
#include "instruction.hpp"

enum class CPUStatus
{
	Carry = 0,
	ZeroResult = 1,
	InterruptDisable = 2,
	DecimalMode = 3,
	BreakCommand = 4,
	Overflow = 5,
	NegativeResult = 6
};

class Core6502
{
	M memory;
	const std::vector<byte> &program;
	byte a, x, y;
	mem_address pc, sp;
	std::bitset<7> status;
	Instruction instruction;

	void setStatus(CPUStatus flag)
	{
		status.set(static_cast<int>(flag));
	}
	void clearStatus(CPUStatus flag)
	{
		status.reset(static_cast<int>(flag));
	}

	// addressing modes
	byte &memImmediate()
	{
		return memory[pc.value + 1];
	}
	byte &memAbsolute()
	{
		return memory[readMemAddress().value];
	}
	byte &memAbsoluteX()
	{
		mem_address addr = readMemAddress();
		byte page = addr.high();
		addr += x;
		byte newPage = addr.high();
		if (newPage > page) instruction.increaseCycles(1);
		return memory[addr.low()];
	}
	byte &memAbsoluteY()
	{
		mem_address addr = readMemAddress();
		byte page = addr.high();
		addr += y;
		byte newPage = addr.high();
		if (newPage > page) instruction.increaseCycles(1);
		return memory[addr.low()];
	}
	byte memZeroPage() const
	{
		return memory[pc.value + 1];
	}
	byte &memZeroPageX() const
	{
		byte val = readByte();
		return memory[val + x];
	}

	// utility methods
	void updateStatusFlags();
	mem_address combine(const byte &highByte, const byte &lowByte) const;
	byte &readByte() const
	{
		return memory[pc.value + 1];
	}
	mem_address readMemAddress() const
	{
		return mem_address(memory[pc.value + 1], memory[pc.value + 2]);
	}

	bool checkBit(const byte &reg, int bitNumber) const
	{
		return reg & (1 << bitNumber);
	}

public:
    Core6502(std::vector<byte> &program);
	
	bool step();
};

#endif /* CORE6502_H */
