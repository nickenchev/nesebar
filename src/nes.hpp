#ifndef NES_H
#define NES_H

#include "core6502.hpp"
#include "mem6502.hpp"
#include "nesmemory.hpp"

class NESCart;

class NES
{
	using Memory = mos6502::Mem6502<NESMemory>;

	const NESCart &cart;
	const NESMemory mapping;
	mos6502::Core<Memory, NESMemory, false> cpu;

public:
	NES(const NESCart &cart);
	void run();
};

#endif /* NES_H */
