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
											  256, 240, 0);
		SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

		if (!window)
		{
			exit(1);
		}

		NESCart cart(path);
		NES nes(cart, renderer);

		bool keepRunning = true;
		while (keepRunning)
		{
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_WINDOWEVENT)
				{
					switch (event.window.event)
					{
						case SDL_WINDOWEVENT_CLOSE:
						{
							keepRunning = false;
							break;
						}
					}
				}
			}

			nes.run();
		}

		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	else
	{
		std::cerr << "ROM file not specified." << std::endl;
	}

    return 0;
}
