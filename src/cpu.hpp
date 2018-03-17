#ifndef CPU_H
#define CPU_H

#include <bitset>
#include <cstdint>

constexpr unsigned int ramSize = 2048;
constexpr unsigned int cpuMemSize = 65535;

using byte = uint8_t;

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
	byte a, x, y, sp, pc;
	std::bitset<7> status;

	void setStatus(CPUStatus flag)
	{
		status.set(static_cast<int>(flag));
	}

	byte memRead(byte address) const;
	void memWrite(byte address, byte data);
	
public:
    CPUCore();
	
	void execue(byte opCode);
};

#endif /* CPU_H */
