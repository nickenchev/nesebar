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
	ZeroResule = 1,
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
	memAddress pc, sp;
	std::bitset<7> status;

	void setStatus(CPUStatus flag)
	{
		status.set(static_cast<int>(flag));
	}
	void clearStatus(CPUStatus flag)
	{
	}

	byte memRead(memAddress address) const;
	void memWrite(memAddress address, byte data);
	memAddress combine(const byte &&highByte, const byte &&lowByte) const;
	
public:
    CPUCore(std::vector<byte> &program);
	
	bool step();
};

#endif /* CPU_H */
