#ifndef NESPPU_H
#define NESPPU_H

#include <SDL2/SDL.h>

class NESPPU
{
	SDL_Renderer *renderer;

public:
	NESPPU(SDL_Renderer *renderer)
	{
		this->renderer = renderer;
	}

	void run()
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawPoint(renderer, 100, 200);

		SDL_RenderPresent(renderer);
	}
};

#endif /* NESPPU_H */
