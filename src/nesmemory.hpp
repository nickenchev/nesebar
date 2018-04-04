#ifndef NESMEMORY_H
#define NESMEMORY_H

#include "memchunk.hpp"

constexpr unsigned int cpuMemSize = 65535;
constexpr unsigned int ppuRegistersSize = 8;
constexpr unsigned int ppuSize = 8184;
constexpr unsigned int apuIORegistersSize = 24;

class NESMemory
{
	MemChunk<byte, cpuMemSize> memory;

public:
	byte memRead(const mem_address &address)
	{
		return memory[address];
	}

	void memWrite(const mem_address &address, byte value)
	{
		if (address < 0x800)
		{
			memory[address] = value;
			memory[address + 0x0800] = value;
			memory[address + 0x1000] = value;
			memory[address + 0x1800] = value;
		}
		else if (address < 0x1000)
		{
			memory[address] = value;
			memory[address - 0x800] = value;
		}
		else if (address < 0x1800)
		{
			memory[address] = value;
			memory[address - 0x1000] = value;
		}
		else if (address < 0x2000)
		{
			memory[address] = value;
			memory[address - 0x1800] = value;
		}
		else if (address < 0x4000)
		{
			mem_address offset = address & 8;
			memory[offset + 0x2000] = value;
		}
	}
};

#endif /* NESMEMORY_H */
