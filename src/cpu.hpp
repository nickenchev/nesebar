#ifndef CPU_H
#define CPU_H

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

class CPUCore
{
	byte *memoryMap[cpuMemSize];
	byte ram[ramSize];
	byte ppuRegisters[ppuRegistersSize];
	byte apuRegisters[apuIORegistersSize];

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
	byte &memImmediate() { return *memoryMap[pc.value + 1]; }
	byte &memAbsolute() { return *memoryMap[readMemAddress().value]; }
	byte &memAbsoluteX()
	{
		mem_address addr = readMemAddress();
		byte page = addr.high();
		addr += x;
		byte newPage = addr.high();
		if (newPage > page) instruction.increaseCycles(1);
		return *memoryMap[addr.low()];
	}
	byte &memAbsoluteY()
	{
		mem_address addr = readMemAddress();
		byte page = addr.high();
		addr += y;
		byte newPage = addr.high();
		if (newPage > page) instruction.increaseCycles(1);
		return *memoryMap[addr.low()];
	}
	byte &memZeroPage() const { return *memoryMap[pc.value + 1]; }
	byte &memZeropageX() const
	{
		const byte &val = readByte();
		return *memoryMap[val + x];
	}

	// utility methods
	void updateStatusFlags();
	mem_address combine(const byte &highByte, const byte &lowByte) const;
	byte &readByte() const { return *memoryMap[pc.value + 1]; }
	mem_address readMemAddress() const
	{
		return mem_address(*memoryMap[pc.value + 1], *memoryMap[pc.value + 2]);
	}
	bool checkBit(int bitNumber) const;
	
public:
    CPUCore(std::vector<byte> &program);
	
	bool step();
};

#endif /* CPU_H */
