#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <bitset>
#include "cpu.h"

using byte = uint8_t;

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

		std::ifstream romFile(path, std::ios::in | std::ios::binary);
		if (romFile.is_open())
		{
			std::cout << "Running: " << path << std::endl;
			Header header;
			romFile.read((char *)&header, sizeof(Header));

			// read flags
			std::bitset<8> flag6(header.flag6);
			std::bitset<8> flag7(header.flag7);

			// reader mapper #
			std::bitset<8> mapperBits(0);
			for (int i = 4; i < 8; ++i)
			{
				if (flag6.test(i))
				{
					mapperBits.set(i);
				}
				if (flag7.test(i))
				{
					mapperBits.set(i + 4);
				}
			}
			byte mapper = static_cast<byte>(mapperBits.to_ulong());

			// check for trainer, read if needed
			bool hasTrainer = flag6.test(flag6Trainer);
			if (hasTrainer)
			{
				byte trainer[trainerSize];
				romFile.read((char *)trainer, trainerSize);
			}

			// load PRG ROM
			const size_t prgRomBytes = prgRomPageSize * header.prgRomSize;
			const size_t chrRomBytes = chrRomPageSize * header.chrRomSize;
			std::vector<byte> prgRom(prgRomBytes);
			std::vector<byte> chrRom(chrRomBytes);

			std::cout << "PRG ROM Size: " << prgRomBytes / 1024 << "KB" << std::endl;
			std::cout << "CHR ROM Size: " << chrRomBytes / 1024 << "KB" << std::endl;

			while (!romFile.eof())
			{
				byte b = 0;
				romFile.read((char *)&b, sizeof(byte));
				//std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)b << ' ';
			}
			romFile.close();
		}
		else
		{
			std::cerr << "Can't load rom file." << std::endl;
		}
	}
	else
	{
		std::cerr << "Can't load rom file." << std::endl;
	}

    return 0;
}
