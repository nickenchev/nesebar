#ifndef NES_H
#define NES_H

#include "nesmemory.hpp"
#include "core6502.hpp"

class NESCart;

class NES
{
	const NESCart &cart;
	NESMemory memory;
	mos6502::Core<NESMemory, false> cpu;

public:
	NES(const NESCart &cart) : cart(cart), memory(cart), cpu(memory) { }
	void run();
};

#endif /* NES_H */
