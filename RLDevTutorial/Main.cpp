#include <iostream>
#include <math.h>
#include <random>
#include <time.h>

#include <SDL.h>

#include "boost/lexical_cast.hpp"

#include "Types.h"

#include "Globals.h"
#include "Color.h"

#include "Entity.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"

#include "MovementListener.h"
#include "RenderingListener.h"

#include "MovementCommand.h"

boost::uuids::random_generator uuidGenerator;

bool quit = false;

int seed = 0;


MovementListener lMovement;
RenderingListener lRendering;

Entity player;
std::map<char, Color> colors;

int main(int argc, char* argv[])
{
	LoadColors("data/color_default.json");

	player.cPhysics = new PhysicsComponent(1, 1);
	player.cRender = new RenderComponent(64, 'y');


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
	RenderEvent e(&player);
	FireEvent(&e);
	
//	Render::PutBorder(0, 0, MAP_WIDTH, MAP_HEIGHT, 'w', 'b', false);
//	Render::PutTitledBorder("Rogue--------------like1", 0, 0, MAP_WIDTH, MAP_HEIGHT, 'w', 'b', false);
	Render::Update();
}

int FireEvent(Event *e)
{
	int r = 0;
	r += lRendering.FireEvent(e);
	r += lMovement.FireEvent(e);
	return r;
}
