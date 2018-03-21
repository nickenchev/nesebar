#ifndef CPU_H
#define CPU_H

#include <bitset>
#include <cstdint>
#include <vector>

constexpr unsigned int cpuMemSize = 65535;
constexpr unsigned int ramSize = 2048;
constexpr unsigned int ppuRegistersSize = 8;
constexpr unsigned int ppuSize = 8184;
constexpr unsigned int apuIORegistersSize = 24;

using byte = uint8_t;
using memAddress = uint16_t;

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
	void end() const { }

	void increaseCycles(short count) { cycles += count; }
	const std::string &getName() const { return name; }
	const short &getStepSize() const { return pcStep; }
	const short &getCycles() const { return cycles; }
};

class CPUCore
{
	byte *memoryMap[cpuMemSize];
	byte ram[ramSize];
	byte ppuRegisters[ppuRegistersSize];
	byte apuRegisters[apuIORegistersSize];

	const std::vector<byte> &program;
	byte A, X, Y;
	memAddress pc, sp;
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
	byte &memImmediate() { return *memoryMap[pc + 1]; }
	byte &memAbsolute() { return *memoryMap[readMemAddress()]; }
	byte &memAbsoluteX()
	{
		memAddress addr = readMemAddress();
		byte page = highByte(addr);
		addr += X;
		byte newPage = highByte(addr);
		if (newPage > page) instruction.increaseCycles(1);
		return *memoryMap[lowByte(addr)];
	}
	byte &memAbsoluteY()
	{
		memAddress addr = readMemAddress();
		byte page = highByte(addr);
		addr += Y;
		byte newPage = highByte(addr);
		if (newPage > page) instruction.increaseCycles(1);
		return *memoryMap[lowByte(addr)];
	}
	byte &memZeroPage() const { return *memoryMap[pc + 1]; }

	// utility methods
	void updateStatusFlags();
	memAddress combine(const byte &highByte, const byte &lowByte) const;
	byte highByte(const memAddress &addr) const { return addr >> 8; }
	byte lowByte(const memAddress &addr) const { return addr & 0xff; };
	byte readByte() const { return *memoryMap[pc + 1]; }
	memAddress readMemAddress() const
	{
		return combine(*memoryMap[pc + 1], *memoryMap[pc + 2]);
	}
	bool checkBit(int bitNumber) const;
	
public:
    CPUCore(std::vector<byte> &program);
	
	bool step();
};

#endif /* CPU_H */
