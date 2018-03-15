#ifndef CPU_H
#define CPU_H

#include <cstdint>

using byte = uint8_t;

class CPUCore
{
	byte a, x, y, sp, pc;
public:
    CPUCore();
	
	void execue(byte opCode);
};

#endif /* CPU_H */
