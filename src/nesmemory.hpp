#ifndef NESMEMORY_H
#define NESMEMORY_H

#include "memchunk.hpp"
#include "nescart.hpp"

class NESMemory
{
	static constexpr unsigned int cpuMemSize = 65535;
	static constexpr unsigned int ppuRegistersSize = 8;
	static constexpr unsigned int ppuSize = 8184;
	static constexpr unsigned int apuIORegistersSize = 24;

	const NESCart &cart;
	MemChunk<byte, cpuMemSize> memory;

public:
	NESMemory(const NESCart &cart) : cart(cart)
	{
		// copy cart ROM into CPU memory directly
		for (mem_address addr = 0; addr < cart.prgRom.size(); ++addr)
		{
			memory[addr + 0x8000] = cart.prgRom[addr.value];
		}
	}

	mem_address memMap(const mem_address &address) const
	{
		mem_address mapped;
		if (address < 0x2000)
		{
			mapped = address % 0x800;
		}
		else if (address < 0x4000)
		{
			// PPU registers
			mapped = address % 0x8 + 0x2000;
		}
		else if (address < 0x4020)
		{
			// APU registers
		}
		else if (address < 0x6000)
		{
			// cartridge expansion ROM
		}
		else if (address < 0x8000)
		{
			// SRAM
		}
		else
		{
			// PRG-ROM
			switch (cart.prgRom.size())
			{
				case 0x8000:
				{
					mapped = address;
					break;
				}
				case 0x4000:
				{
					mapped = address % 0x4000 + 0x8000;
					break;
				}
			}
		}

		return mapped;
	}

	byte memRead(const mem_address &address)
	{
		return memory[memMap(address)];
	}

	void memWrite(const mem_address &address, byte value)
	{
		memory[memMap(address)] = value;
	}
};

#endif /* NESMEMORY_H */
