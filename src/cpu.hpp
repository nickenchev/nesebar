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

class StepSize
{
	short pcStep;
	short cycles;

public:
	StepSize()
	{
		pcStep = 0;
		cycles = 0;
	}

	void setup(short pcStep, short cycles)
	{
		this->pcStep = pcStep;
		this->cycles = cycles;
	}

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
	StepSize stepSize;

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
	byte &memAbsoluteX() { return *memoryMap[readMemAddress() + X]; }
	byte &memAbsoluteY() { return *memoryMap[readMemAddress() + Y]; }
	byte &memZeroPage() { return *memoryMap[pc + 1]; }
	byte &memIndexed(const byte &reg) { return *memoryMap[pc + reg]; }
	byte &memIndexedX() { return memIndexed(X); }
	byte &memIndexedY() { return memIndexed(Y); }


	// utility methods
	byte highByte(const memAddress &addr) const;
	byte lowByte(const memAddress &addr) const;
	byte readByte() const { return *memoryMap[pc + 1]; }
	memAddress readMemAddress() const
	{
		return combine(*memoryMap[pc + 1], *memoryMap[pc + 2]);
	}
	memAddress combine(const byte &highByte, const byte &lowByte) const;
	bool checkBit(int bitNumber) const;
	void updateStatusFlags();
	
public:
    CPUCore(std::vector<byte> &program);
	
	bool step();
};

#endif /* CPU_H */
