#include <iostream>
#include <math.h>
#include <random>
#include <time.h>

#include <SDL.h>

#include "boost/lexical_cast.hpp" 
#include "Globals.h"

#include "Entity.h"
#include "PhysicsComponent.h"

#include "MovementListener.h"

#include "MovementCommand.h"

boost::uuids::random_generator uuidGenerator;

bool quit = false;

int seed = 0;


MovementListener lMovement;

Entity player;

int main(int argc, char* argv[])
{
	player.cPhysics = new PhysicsComponent(1, 1);


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

		Command *command = nullptr;

		while (SDL_PollEvent(&input))
		{
			if (input.type == SDL_KEYDOWN)
			{
				switch (input.key.keysym.sym)
				{
					case SDLK_UP:
						command = new MovementCommand(&player, 0, -1);
						break;
					case SDLK_DOWN:
						command = new MovementCommand(&player, 0, 1);
						break;
					case SDLK_RIGHT:
						command = new MovementCommand(&player, 1, 0);
						break;
					case SDLK_LEFT:
						command = new MovementCommand(&player, -1, 0);
						break;
					default:
						break;
				}
			}
		}

		if (command != nullptr)
		{
			command->Execute();
			delete command;
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
	Render::Put(64, player.cPhysics->x, player.cPhysics->y, 255, 255, 255);
	Render::Update();
}

int FireEvent(Event *e)
{
	int r = 0;

	r += lMovement.FireEvent(e);


	return r;
}
