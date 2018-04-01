#include <vector>
#include <iostream>
#include <string>
#include <memory>

#include "nes.hpp"
#include "nescart.hpp"

int main(int argc, const char *argv[])
{
	if (argc > 1 && argv[1] != nullptr)
	{
		std::string path(argv[1]);
		std::cout << "ROM File: " << path << std::endl;

		auto cart = NESCart::load(path);

		NES nes(cart.get());
		nes.start();
	}
	else
	{
		std::cerr << "ROM file not specified." << std::endl;
	}

    return 0;
}
