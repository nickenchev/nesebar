#ifndef CPU_H
#define CPU_H

#include <bitset>
#include <cstdint>
#include <vector>

constexpr unsigned int ramSize = 2048;
constexpr unsigned int cpuMemSize = 65535;

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
	byte ram[ramSize];
	byte *memoryMap[cpuMemSize];
	const std::vector<byte> &program;
	byte a, x, y, sp, pc;
	std::bitset<7> status;

	void setStatus(CPUStatus flag)
	{
		status.set(static_cast<int>(flag));
	}
	void clearStatus(CPUStatus flag)
	{
	}

	byte memRead(byte address) const;
	void memWrite(byte address, byte data);
	
public:
    CPUCore(const std::vector<byte> &program);
	
	bool step();
};

#endif /* CPU_H */
