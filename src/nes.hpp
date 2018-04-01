#ifndef NES_H
#define NES_H

#include <iomanip>
#include <bitset>
#include <memory>

#include "nesmemory.hpp"
#include "core6502.hpp"

constexpr int prgRomPageSize = 16384;
constexpr int chrRomPageSize = 8192;
constexpr int trainerSize = 512;
constexpr int flag6FourScreenMode = 3;
constexpr int flag6Trainer = 2;
constexpr int flag6SRAMBattery = 1;
constexpr int flag6Mirroring = 0;

struct Header
{
	char code[4];
	byte prgRomSize;
	byte chrRomSize;
	byte flag6;
	byte flag7;
	byte prgRamSize;
	byte flag9;
	byte flag10;
	byte zeroFilled[5];

	Header()
	{
		for (int i = 0; i < 4; ++i) code[i] = 0;
		prgRomSize = 0;
		chrRomSize = 0;
		flag6 = flag7 = flag9 = flag10 = 0;
		for (int i = 0; i < 5; ++i) zeroFilled[i] = 0;
	}
};

struct NESCart
{
	std::vector<byte> prgRom;
	std::vector<byte> chrRom;

	NESCart(size_t prgSize, size_t chrSize) : prgRom(prgSize), chrRom(chrSize)
	{
	}
};

class NES
{
	Core6502<NESMemory> cpu;

public:
	static std::unique_ptr<NESCart> load(const std::string &romPath);

	void run(NESCart *cart)
	{
		while (cpu.step()) { }
	}
};

#endif /* NES_H */
