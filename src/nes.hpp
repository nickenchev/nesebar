#ifndef NES_H
#define NES_H

#include "nesmemory.hpp"
#include "core6502.hpp"

class NESCart;

class NES
{
	const NESCart &cart;
	mos6502::Core<NESMemory> cpu;

public:
	NES(const NESCart &cart) : cart(cart) { }

	void start()
	{
		while (cpu.step()) { }
	}
};

#endif /* NES_H */
