#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <SDL2/SDL.h>

#include "nes.hpp"
#include "nescart.hpp"

int main(int argc, const char *argv[])
{
	if (argc > 1 && argv[1] != nullptr)
	{
		std::string path(argv[1]);
		std::cout << "ROM File: " << path << std::endl;

		SDL_Init(SDL_INIT_VIDEO);
		SDL_Window *window = SDL_CreateWindow("Nesebar", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
											  256, 240, SDL_WINDOW_OPENGL);

		if (!window)
		{
			exit(1);
		}

		NESCart cart(path);
		NES nes(cart);
		nes.run();
	}
	else
	{
		std::cerr << "ROM file not specified." << std::endl;
	}

    return 0;
}
