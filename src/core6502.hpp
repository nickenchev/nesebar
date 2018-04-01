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

template<typename M>
class Core6502
{
	M memory;
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

	void updateStatusFlags();
	bool checkBit(const byte &reg, int bitNumber) const
	{
		return reg & (1 << bitNumber);
	}

	byte readByte()
	{
		return memory.memRead(pc + 1);
	}
	mem_address readMemAddress()
	{
		return mem_address(memory.memRead(pc + 1), memory.memRead(pc + 2));
	}
	/*
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
	*/

public:
    Core6502()
	{
		x = y = a = 0;
	}
	
	bool step();
};

#endif /* CORE6502_H */
