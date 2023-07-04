#include <iostream>
#include <math.h>
#include <random>
#include <time.h>

#include <SDL.h>

#include "boost/lexical_cast.hpp" 
#include "Globals.h"

boost::uuids::random_generator uuidGenerator;

bool quit = false;

int seed = 0;

int pX, pY;

int main(int argc, char* argv[])
{
	pX = 0;
	pY = 0;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
		return 1;
	}
	seed = time(NULL);
	Render::Init();
	while (!quit)
	{
		SDL_Event input;

		while (SDL_PollEvent(&input))
		{
			if (input.type == SDL_KEYDOWN)
			{
				switch (input.key.keysym.sym)
				{
					case SDLK_UP:
						--pY;
						break;
					case SDLK_DOWN:
						++pY;
						break;
					case SDLK_RIGHT:
						++pX;
						break;
					case SDLK_LEFT:
						--pX;
						break;
					default:
						break;
				}
			}
		}
		RenderAll();
	}
	return 0;
}

std::string GenerateUUID()
{
	return boost::lexical_cast<std::string>(uuidGenerator());
}

void RenderAll()
{
	Render::Put(64, pX, pY, 255, 255, 255);
	Render::Update();
}