#ifndef NESMEMORY_H
#define NESMEMORY_H

#include "memchunk.hpp"
#include "mappedaddress.hpp"
#include "nescart.hpp"

class NESMemory
{
	static constexpr unsigned int ppuRegistersSize = 8;
	static constexpr unsigned int ppuSize = 8184;
	static constexpr unsigned int apuIORegistersSize = 24;

	const NESCart &cart;

public:
	NESMemory(const NESCart &cart) : cart(cart) {}

	MappedAddress mapAddress(const MemAddress &address) const
	{
		MappedAddress mapped;
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
					mapped = {address, true};
					break;
				}
				case 0x4000:
				{
					mapped = {address % 0x4000 + 0x8000, true};
					break;
				}
			}
		}
		return mapped;
	}
};

#endif /* NESMEMORY_H */
