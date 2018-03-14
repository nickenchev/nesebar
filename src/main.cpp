#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "cpu.h"

using byte = uint8_t;

struct Header
{
	char code[3];
	byte prgRomSize;
	byte chrRomSize;
	byte flag6;
	byte flag7;
	byte prgRamSize;
	byte flag9;
	byte flag10;
	byte zeroFilled[5];
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
