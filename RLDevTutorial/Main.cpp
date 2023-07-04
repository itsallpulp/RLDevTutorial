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

int main(int argc, char* argv[])
{
	

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
		return 1;
	}
	seed = time(NULL);
	Render::Init();
	while (!quit)
	{
		
	}
	return 0;
}

std::string GenerateUUID()
{
	return boost::lexical_cast<std::string>(uuidGenerator());
}

void RenderAll()
{
	
}