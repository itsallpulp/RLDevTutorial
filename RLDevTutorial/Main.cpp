#include <chrono>
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
#include "LogListener.h"
#include "MovementListener.h"
#include "RenderingListener.h"

#include "MovementCommand.h"

#include "Level.h"

#include "Pathfinder.h"

#include "EntityManager.h"


boost::uuids::random_generator uuidGenerator;

bool quit = false;

int seed = 0;

EntityManager *actorManager;

FOVListener lFOV;
LogListener lLog;
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

void RenderEntity(Entity *e);
void RenderHUD(Entity *e);

void PrintRuntime(void (*func)(void));

std::map<std::string, json::object> jsonCache;
std::vector<std::string> jsonCacheKey;

int main(int argc, char* argv[])
{
	WeightedBag<std::string> wb = WeightedBagFromJSON(GetJson("table_monsters"));

	actorManager = new EntityManager();
	exploreDestination = { -1, -1 };
	seed = time(NULL);
	srand(seed);

	LoadColors("data/color_default.json");
	level = new Level();
	level->RoomsAndMazes();

	int playerIndex = actorManager->AddEntity("player");
	player = actorManager->GetEntity(playerIndex);

	player->cPhysics->x = 1;
	player->cPhysics->y = 1;

	level->PlaceEntity(player);

	for (int i = 0; i < 20; ++i)
	{
		int monsterIndex = actorManager->AddEntity(wb.GetRandomValue());
		Entity *monster = actorManager->GetEntity(monsterIndex);
		std::cout << monster->GetName() << std::endl;
		level->PlaceEntity(monster);
	}

	lFOV.DoFOV(player);

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
		
			while (SDL_PollEvent(&input))
			{
				if (input.type == SDL_KEYDOWN)
				{
					autoExploring = false;
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
							//level->FromLevelConfig(LGNystromRoomsAndMazes(MAP_WIDTH, MAP_HEIGHT).GenerateLevel());
							level->RoomsAndMazes();
							level->PlaceEntity(player);
							lFOV.DoFOV(player);
							break;
						case SDLK_x:
							autoExploring = true;
							break;
						case SDLK_r:
							level->RevealAll();
							break;
						default:
							break;
					}
				}
			}

			if(command == nullptr && autoExploring)
			{
				//SDL_Delay(25);
				autoExploring = AutoExplore();
			}
			else if (command != nullptr)
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
	level->Render();
	actorManager->RunFunc(&RenderEntity);
	RenderHUD(player);
	/*
	for (Entity *ent : actorManager.GetEntities())
	{
		RenderEvent e(ent);
		FireEvent(&e);
	}*/

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
	r += lLog.FireEvent(e);
	return r;
}

json::object GetJson(std::string filename)
{
	if (jsonCache.find(filename) != jsonCache.end())
	{
		return jsonCache[filename];
	}

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
	jsonCache[filename] = data.as_object();
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

WeightedBag<std::string> WeightedBagFromJSON(json::object data)
{
	/* Data should be written as a table 
	IE
	{
		"actor_orc" : 3,
		"actor_troll": 1,
		"table_vermin": 1
	}
	
	*/

	WeightedBag<std::string> bag;

	for (auto item : data)
	{
		bag.AddValue(item.key(), json::value_to<int>(item.value()));
	}

	return bag;
}

bool AutoExplore()
{
	for (Entity *ent : actorManager->GetEntities())
	{
		if (ent == player) { continue; }
		point p = ent->GetXY();

		if (level->GetFOV(p.first, p.second) == fovVisible)
		{
			LogEvent logEvent(player, "You stop exploring because you see a " + ent->GetName() +".");
			FireEvent(&logEvent);
			return false;
		}

	}

	std::vector<point> unvisited;
	for (int x = 1; x < MAP_WIDTH - 1; ++x)
	{
		for (int y = 1; y < MAP_HEIGHT - 1; ++y)
		{
			if (level->GetFOV(x, y) != fovHidden /* || level->GetCell(x, y)->BlocksMovement()*/) { continue; }
			unvisited.push_back({ x,y });
		}
	}

	if (unvisited.size() == 0) {
		std::cout << "Nowhere to go!" << std::endl;
		/*level->RoomsAndMazes(10);
		level->PlaceEntity(player);
		lFOV.DoFOV(player);*/
		return true; 
	}

	int **path = pathfinder->CreateDijkstraMap(unvisited);

	int dx = 0, dy = 0;
	point p = player->GetXY();
	int value = path[p.first][p.second];

	if (path[p.first + 1][p.second] < value) { dx = 1; dy = 0; value = path[p.first + dx][p.second + dy]; }
	if (path[p.first - 1][p.second] < value) { dx = -1; dy = 0; value = path[p.first + dx][p.second + dy]; }
	if (path[p.first][p.second + 1] < value) { dx = 0; dy = 1;  value = path[p.first + dx][p.second + dy];}
	if (path[p.first][p.second - 1] < value) { dx = 0; dy = -1; value = path[p.first + dx][p.second + dy];}
	
	if (dx == 0 && dy == 0)
	{
		SDL_Delay(500);
		/*level->RoomsAndMazes(250);
		level->PlaceEntity(player);
		lFOV.DoFOV(player);*/
		return false;
	}

	MovementCommand c(player, dx, dy);
	c.Execute();
	SDL_Delay(25);
	return true;
}

void RenderEntity(Entity *e)
{
	if (level->GetFOV(e->GetXY()) != fovVisible) { return; }

	RenderEvent ev(e);
	FireEvent(&ev);
}

void PrintRuntime(void(*func)(void))
{
	auto start = std::chrono::high_resolution_clock::now();
	func();
	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << " ms" << std::endl;
}

void RenderHUD(Entity *e)
{
	Render::PutTitledBorder("Player", 0, MAP_HEIGHT, MAP_WIDTH, GUI_HEIGHT, 'w', 'x', BORDER_TITLE_LEFT | FILL_BACKGROUND);
	/* Draw player logs */
	if (e->cLog != nullptr)
	{
		int y = MAP_HEIGHT + 1,
			x = 15;
		for (std::string log : e->cLog->logs)
		{
			Render::Puts(log, x, y++, 'w');
		}
	}
	Render::PutTitledBorder("Player", 0, MAP_HEIGHT, MAP_WIDTH, GUI_HEIGHT, 'w', 'x', BORDER_TITLE_LEFT);
}