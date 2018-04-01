#ifndef NES_H
#define NES_H

#include "nesmemory.hpp"
#include "core6502.hpp"

class NESCart;

class NES
{
	Core6502<NESMemory> cpu;

public:
	void run(NESCart *cart)
	{
		while (cpu.step()) { }
	}
};

#endif /* NES_H */
