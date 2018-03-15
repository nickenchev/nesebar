#ifndef CPU_H
#define CPU_H

#include <bitset>
#include <cstdint>

constexpr unsigned int ramSize = 2048;

using byte = uint8_t;

enum class CPUStatus
{
	Carry = 0,
	Zero = 1,
	InterruptDisable = 2,
	DecimalMode = 3,
	Break = 4,
	Overflow = 5,
	Negative = 6
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
