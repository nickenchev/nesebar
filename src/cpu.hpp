#ifndef CPU_H
#define CPU_H

#include <bitset>
#include <cstdint>

constexpr unsigned int ramSize = 2048;

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

enum class OpCode
{
	BRK = 0x00,
	ORA = 0x01,
	STP = 0x02,
};

class CPUCore
{
	byte a, x, y, sp, pc;
	byte ram[ramSize];
	std::bitset<7> status;

	void setStatus(CPUStatus flag)
	{
		status.set(static_cast<int>(flag));
	}
	
public:
    CPUCore();
	
	void execue(byte opCode);
};

#endif /* CPU_H */
