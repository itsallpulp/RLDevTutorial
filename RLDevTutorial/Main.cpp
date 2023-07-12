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

#include "FOVListener.h"
#include "MovementListener.h"
#include "RenderingListener.h"

#include "MovementCommand.h"

#include "Level.h"

#include "Pathfinder.h"

#include "EntityManager.h"

boost::uuids::random_generator uuidGenerator;

bool quit = false;

int seed = 0;

EntityManager actorManager;

FOVListener lFOV;
MovementListener lMovement;
RenderingListener lRendering;

Entity *player;
Level *level;
std::map<char, Color> colors;

bool autoExploring;
Pathfinder *pathfinder = new Pathfinder();
bool AutoExplore();
point exploreDestination;
std::stack<point> explorePath;

int main(int argc, char* argv[])
{
	exploreDestination = { -1, -1 };
	seed = time(NULL);
	srand(seed);

	LoadColors("data/color_default.json");
	level = new Level();
	level->RoomsAndMazes();

	int playerIndex = actorManager.AddEntity("player");
	std::cout << playerIndex << std::endl;
	player = actorManager.GetEntity(playerIndex);

	player->cPhysics->x = 1;
	player->cPhysics->y = 1;

	level->PlaceEntity(player);

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
		return 1;
	}
	Render::Init();
	while (!quit)
	{
		SDL_Event input;

		Command *command = nullptr;

		{
			while (SDL_PollEvent(&input))
			{
				if (input.type == SDL_KEYDOWN)
				{
					switch (input.key.keysym.sym)
					{
						case SDLK_UP:
							command = new MovementCommand(player, 0, -1);
							break;
						case SDLK_DOWN:
							command = new MovementCommand(player, 0, 1);
							break;
						case SDLK_RIGHT:
							command = new MovementCommand(player, 1, 0);
							break;
						case SDLK_LEFT:
							command = new MovementCommand(player, -1, 0);
							break;
						case SDLK_z:
							command = new MovementCommand(player, 0, 0);
							break;
						case SDLK_c:
							level->RoomsAndMazes();
							level->PlaceEntity(player);
							break;
						case SDLK_x:
							autoExploring = true;
							break;
						default:
							break;
					}
				}
			}

			if(command == nullptr && autoExploring)
			{
				SDL_Delay(45);
				autoExploring = AutoExplore();
			}
			else if (command != nullptr)
			{
				autoExploring = false;
				command->Execute();
				delete command;
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
	level->Render();

	for (Entity *ent : actorManager.GetEntities())
	{
		RenderEvent e(ent);
		FireEvent(&e);
	}

//	Render::PutBorder(0, 0, MAP_WIDTH, MAP_HEIGHT, 'w', 'b', false);
//	Render::PutTitledBorder("Rogue--------------like1", 0, 0, MAP_WIDTH, MAP_HEIGHT, 'w', 'b', false);
	Render::Update();
}

int FireEvent(Event *e)
{
	int r = 0;
	r += lRendering.FireEvent(e);
	r += lMovement.FireEvent(e);
	r += lFOV.FireEvent(e);
	return r;
}

json::object GetJson(std::string filename)
{
	json::value data;
	std::ifstream inf("data/" + filename + ".json");
	std::string s = "";
	json::stream_parser p;
	json::error_code ec;
	while (std::getline(inf, s))
	{
		p.write(s, ec);
		if (ec)
		{
			return json::object();
		}
	}

	data = p.release();
	return data.as_object();	
}

double Distance(point start, point end)
{
	return std::sqrt( std::pow(end.second - start.second, 2) + std::pow(end.first - start.first, 2));
}

MovementCommand *FollowPath(Entity *target, std::stack<point> *path)
{
	point p = path->top(), t = target->GetXY();
	path->pop();

	return new MovementCommand(target, p.first - t.first, p.second - t.second);
}

bool AutoExplore()
{
	std::vector<point> unvisited;

	for (int x = 1; x < MAP_WIDTH - 1; ++x)
	{
		for (int y = 1; y < MAP_HEIGHT - 1; ++y)
		{
			if (level->GetFOV(x, y) != fovHidden /* || level->GetCell(x, y)->BlocksMovement()*/) { continue; }
			unvisited.push_back({ x,y });
		}
	}

	if (unvisited.size() == 0) { return false; }

	int **path = pathfinder->CreateDijkstraMap(unvisited);

	int dx = 0, dy = 0;
	point p = player->GetXY();
	int value = path[p.first][p.second];

	if (path[p.first + 1][p.second] < value) { dx = 1; dy = 0; value = path[p.first + dx][p.second + dy]; }
	if (path[p.first - 1][p.second] < value) { dx = -1; dy = 0; value = path[p.first + dx][p.second + dy]; }
	if (path[p.first][p.second + 1] < value) { dx = 0; dy = 1;  value = path[p.first + dx][p.second + dy];}
	if (path[p.first][p.second - 1] < value) { dx = 0; dy = -1; value = path[p.first + dx][p.second + dy];}
	
	MovementCommand c(player, dx, dy);
	c.Execute();

	return true;
}
