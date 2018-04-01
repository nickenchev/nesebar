#include <fstream>
#include <iterator>
#include <iostream>

#include "nes.hpp"

std::unique_ptr<NESCart> NES::load(const std::string &romPath)
{
	NESCart *cart = nullptr;
	std::ifstream romFile(romPath, std::ios::in | std::ios::binary);
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
		byte mapperNum = ((flag6bits >> 4) & 0b00001111)
			| (flag7bits & 0b11110000);
		std::cout << "Read Mapper #" << std::setw(2)
					<< std::setfill('0') << (int)mapperNum << std::endl;

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
		std::cout << "PRG ROM Size: " << prgRomBytes
					<< " Bytes" << std::endl;
		std::cout << "CHR ROM Size: " << chrRomBytes
					<< " Bytes" << std::endl;

		cart = new NESCart(prgRomBytes, chrRomBytes);
		cart->prgRom.insert(cart->prgRom.begin(),
							std::istream_iterator<byte>(romFile),
							std::istream_iterator<byte>());

		romFile.close();
	}
	else
	{
		std::cerr << "Can't load ROM file." << std::endl;
	}

	return std::unique_ptr<NESCart>(cart);
}
