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
using signed_byte = int8_t;

union mem_address
{
	byte bytes[2];
	uint16_t value;

	mem_address(byte low, byte high)
	{
		bytes[0] = low;
		bytes[1] = high;
	}
	mem_address(uint16_t value) : value(value) { }
	mem_address() : value(0) { }

	byte low() const { return bytes[0]; }
	byte high() const { return bytes[1]; }

	mem_address &operator=(uint16_t value)
	{
		this->value = value;
		return *this;
	}

	bool operator<=(const uint16_t &value) const
	{
		return this->value <= value;
	}

	mem_address operator+(int) const
	{
		mem_address addr = *this;
		addr.value += value;
		return addr;
	}

	mem_address operator+(byte value) const
	{
		mem_address addr = *this;
		addr.value += value;
		return addr;
	}

	mem_address &operator+=(byte value)
	{
		this->bytes[0] += value;
		return *this;
	}

	mem_address &operator+=(uint16_t value)
	{
		this->value += value;
		return *this;
	}

	bool operator<(const mem_address &rhs) const
	{
		return this->value < rhs.value;
	}
};

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
