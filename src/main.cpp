#include <vector>
#include <iostream>
#include <string>
#include <memory>

#include "nes.hpp"


int main(int argc, const char *argv[])
{
	if (argc > 1 && argv[1] != nullptr)
	{
		std::string path(argv[1]);
		std::cout << "ROM File: " << path << std::endl;

		auto cart = NES::load(path);

		NES nes;
		nes.run(cart.get());
	}
	else
	{
		std::cerr << "Can't load ROM file." << std::endl;
	}

    return 0;
}
