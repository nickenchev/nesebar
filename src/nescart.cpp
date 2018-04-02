#include <fstream>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <memory>

#include "nescart.hpp"

NESCart::NESCart(const std::string &romPath)
{
	std::ifstream romFile(romPath, std::ios::in | std::ios::binary);
	if (romFile.is_open())
	{
		// load iNES header
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
		byte mapperNum = ((flag6bits >> 4) & 0b00001111)
			| (flag7bits & 0b11110000);
		std::cout << "Read Mapper #" << std::setw(2)
					<< std::setfill('0') << (int)mapperNum << std::endl;

		// check flag6 for trainer, read if needed
		bool hasTrainer = flag6.test(flag6Trainer);
		if (hasTrainer)
		{
			// throw away for now
			byte trainer[trainerSize];
			romFile.read((char *)trainer, trainerSize);
		}

		const size_t prgRomBytes = prgRomPageSize * header.prgRomSize;
		const size_t chrRomBytes = chrRomPageSize * header.chrRomSize;
		prgRom.resize(prgRomBytes);
		chrRom.resize(chrRomBytes);
		std::cout << "PRG ROM Size: " << prgRomBytes << " bytes\n"
				  << "CHR ROM Size: " << chrRomBytes << " bytes" << std::endl;

		romFile.read((char *)&prgRom[0], prgRomBytes);
		romFile.read((char *)&chrRom[0], chrRomBytes);

		romFile.close();
	}
	else
	{
		std::cerr << "Can't load ROM file." << std::endl;
	}
}
