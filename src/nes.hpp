#ifndef NES_H
#define NES_H

#include "core6502.hpp"
#include "mem6502.hpp"
#include "nesmemory.hpp"
#include "nesppu.hpp"

class NES
{
	using Memory = mos6502::Mem6502<NESMemory>;

	const NESMemory mapping;
	mos6502::Core<Memory, NESMemory, false> cpu;
	NESPPU ppu;

public:
	NES(const NESCart &cart, SDL_Renderer *renderer);
	void run();
};

#endif /* NES_H */
