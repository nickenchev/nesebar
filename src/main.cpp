#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <bitset>

#include "cpu.hpp"

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

int main(int argc, const char *argv[])
{
	if (argc > 1 && argv[1] != nullptr)
	{
		std::string path(argv[1]);
		std::cout << "ROM File: " << path << std::endl;

		std::ifstream romFile(path, std::ios::in | std::ios::binary);
		if (romFile.is_open())
		{
			// load iNES header
			Header header;
			romFile.read((char *)&header, sizeof(Header));
			if (header.code[0] == 'N' && header.code[1] == 'E' &&
				header.code[2] == 'S' && header.code[3] == 0x1A)
			{
				std::cout << "iNES OK" << std::endl;
			}
			else
			{
				std::cout << "iNES Error" << std::endl;
			}

			// read flags
			std::bitset<8> flag6(header.flag6);
			std::bitset<8> flag7(header.flag7);

			// reader mapper #
			byte flag6bits = static_cast<byte>(flag6.to_ulong());
			byte flag7bits = static_cast<byte>(flag7.to_ulong());
			byte mapperNum = ((flag6bits >> 4) & 0b00001111) | (flag7bits & 0b11110000);
			std::cout << "Read Mapper #" << std::setw(2) << std::setfill('0') << (int)mapperNum << std::endl;

			// check flag6 for trainer, read if needed
			bool hasTrainer = flag6.test(flag6Trainer);
			if (hasTrainer)
			{
				byte trainer[trainerSize];
				romFile.read((char *)trainer, trainerSize);
			}

			// load PRG ROM
			const size_t prgRomBytes = prgRomPageSize * header.prgRomSize;
			const size_t chrRomBytes = chrRomPageSize * header.chrRomSize;
			std::cout << "PRG ROM Size: " << prgRomBytes << " Bytes" << std::endl;
			std::cout << "CHR ROM Size: " << chrRomBytes << " Bytes" << std::endl;
			std::vector<byte> prgRom(prgRomBytes);
			std::vector<byte> chrRom(chrRomBytes);
			prgRom.insert(prgRom.begin(),
						  std::istream_iterator<byte>(romFile),
						  std::istream_iterator<byte>());

			CPUCore cpu(prgRom);
			while (cpu.step()) { }

			romFile.close();
		}
		else
		{
			std::cerr << "Can't load ROM file." << std::endl;
		}
	}
	else
	{
		std::cerr << "Can't load ROM file." << std::endl;
	}

    return 0;
}
