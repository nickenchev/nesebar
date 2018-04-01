#ifndef NES_H
#define NES_H

#include "nesmemory.hpp"
#include "core6502.hpp"

class NESCart;

class NES
{
	const NESCart *cart;
	Core6502<NESMemory> cpu;

public:
	NES(NESCart *cart)
	{
		this->cart = cart;
	}

	void start()
	{
		while (cpu.step()) { }
	}
};

#endif /* NES_H */
