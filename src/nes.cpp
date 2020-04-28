#include "nes.hpp"

NES::NES(const NESCart &cart) : cart(cart), mapping(cart), cpu(mapping)
{
	// copy cart ROM into CPU memory directly
	for (MemAddress addr = 0; addr < cart.prgRom.size(); ++addr)
	{
		Memory &memory = cpu.getMemory();
		const MemAddress romStart = 0x8000;
		memory[addr + romStart] = cart.prgRom[addr.value];
	}
}

void NES::run()
{
	cpu.reset();
	while (cpu.step())
	{
	}
}
