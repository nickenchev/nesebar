#include "nes.hpp"

NES::NES(const NESCart &cart, SDL_Renderer *renderer) : mapping(cart), cpu(mapping), ppu(renderer)
{
	// copy cart ROM into CPU memory directly
	for (MemAddress addr = 0; addr < cart.prgRom.size(); ++addr)
	{
		Memory &memory = cpu.getMemory();
		const MemAddress romStart = 0x8000;
		memory[addr + romStart] = cart.prgRom[addr.value];
	}
	cpu.reset();
}

void NES::run()
{
	cpu.step();
	ppu.run();
}
