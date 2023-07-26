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

#include "CombatListener.h"
#include "DeathListener.h"
#include "FOVListener.h"
#include "LogListener.h"
#include "MovementListener.h"
#include "RenderingListener.h"
#include "TurnListener.h"

#include "MovementCommand.h"

#include "Level.h"

#include "Pathfinder.h"

#include "EntityManager.h"


boost::uuids::random_generator uuidGenerator;

bool quit = false;

int seed = 0;

EntityManager *actorManager;

CombatListener lCombat;
DeathListener lDeath;
FOVListener lFOV;
LogListener lLog;
MovementListener lMovement;
RenderingListener lRendering;
TurnListener lTurn;

Entity *player;
Level *level;
std::map<char, Color> colors;

Pathfinder *pathfinder = new Pathfinder();
int gameState = ON_MAP;

void RenderEntity(Entity *e);
void RenderHUD(Entity *e);
void RenderFloatingText(FloatingText *text);

void TakeTurn(Entity *actor);

void PrintRuntime(void (*func)(void));

std::map<std::string, json::object> jsonCache;
std::vector<std::string> jsonCacheKey;
std::vector<FloatingText> *floatingTexts = new std::vector<FloatingText>();

int main(int argc, char* argv[])
{
	WeightedBag<std::string> wb = WeightedBagFromJSON(GetJson("table_monsters"));

	actorManager = new EntityManager();
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
		/* Do turn loop */

		actorManager->RunFunc(TakeTurn);

		/*
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
				autoExploring = AutoExplore();
			}
			else if (command != nullptr)
			{
				command->Execute();
				delete command;
			}
		
		
		RenderAll();
		*/
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

	for (auto it = floatingTexts->begin(); it != floatingTexts->end();)
	{
		if ((*it).opacity == 0)
		{
			it = floatingTexts->erase(it);
		}
		else
		{
			RenderFloatingText(&(*it));
			++it;
		}
	}

	Render::Update();
}

int WorldFireEvent(Event *e)
{
	int r = 0;
	r += lRendering.FireEvent(e);
	r += lMovement.FireEvent(e);
	r += lFOV.FireEvent(e);
	r += lLog.FireEvent(e);
	r += lCombat.FireEvent(e);
	r += lTurn.FireEvent(e);
	r += lDeath.FireEvent(e);
	return r;
}

json::object GetJson(std::string filename)
{
	for (auto it = jsonCacheKey.begin(); it != jsonCacheKey.end(); ++it)
	{
		if ((*it) == filename)
		{
			jsonCacheKey.erase(it);
			jsonCacheKey.emplace(jsonCacheKey.begin(), filename);
			return jsonCache[filename];
		}
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
	jsonCacheKey.emplace(jsonCacheKey.begin(), filename);

	while (jsonCacheKey.size() > 200)
	{
		std::string key = (*jsonCacheKey.end());
		jsonCache.erase(key);
		jsonCacheKey.pop_back();
	}

	return data.as_object();	
}

double Distance(point start, point end)
{
	return std::sqrt( std::pow(end.second - start.second, 2) + std::pow(end.first - start.first, 2));
}

void AddFloatingText(std::string text, char color, int x, int y, int speed)
{
	if (level->GetFOV(x, y) != fovVisible) { return; }

	FloatingText t;
	t.msg = text;
	t.color = color;
	t.x = (x * SPRITE_WIDTH) - ((text.size() / 2) * SPRITE_WIDTH) + (SPRITE_WIDTH / 2);
	t.y = ((y - 1) * SPRITE_HEIGHT);
	t.opacity = 255;
	t.ticks = 0;
	t.speed = speed;
	floatingTexts->push_back(t);
}

bool CanSee(point start, point end)
{
	int deltaX = end.first - start.first;
	signed char const ix = ((deltaX > 0) - (deltaX < 0));
	deltaX = std::abs(deltaX) << 1;

	int deltaY = end.second - start.second;
	signed char const iy = ((deltaY > 0) - (deltaY < 0));
	deltaY = std::abs(deltaY) << 1;

	int maxMonsterSight = 15;

	int X2 = end.first,
		X1 = start.first,
		Y2 = end.second,
		Y1 = start.second,
		count = 0;

	if (deltaX >= deltaY)
	{
		int error = deltaY - (deltaX >> 1);


		while (X1 != X2)
		{
			if ((error > 0) || (!error && (ix > 0)))
			{
				error -= deltaX;
				Y1 += iy;
			}

			error += deltaY;
			X1 += ix;
			++count;

			if (level->GetCell(X1, Y1)->BlocksVision() || count > 25) { return false; }
		}
	}
	else
	{
		int error = deltaX - (deltaY >> 1);

		while (Y1 != Y2)
		{
			if ((error > 0) || (!error && (iy > 0)))
			{
				error -= deltaY;
				X1 += ix;
			}

			error += deltaX;
			Y1 += iy;
			++count;

			if (level->GetCell(X1, Y1)->BlocksVision() || count > 25) { return false; }
		}
	}

	return true;
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

int Roll(int numDice, int dieSize)
{
	int n = 0;

	for (int i = 0; i < numDice; ++i)
	{
		n += (rand() % dieSize) + 1;
	}

	return n;
}

int RollDamage(int numDice)
{
	int damages[] = { 0, 0, 1, 2, 1, 1 };
	int n = 0;

	for (int i = 0; i < numDice; ++i)
	{
		n += damages[rand() % 6];
	}

	return n;
}

void RenderEntity(Entity *e)
{
	if (level->GetFOV(e->GetXY()) != fovVisible) { return; }

	RenderEvent ev(e);
	WorldFireEvent(&ev);
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
	std::string title = "HP: " + std::to_string(e->GetHealth()) + "/" + std::to_string(e->GetMaxHealth());

	Render::PutTitledBorder("Player", 0, MAP_HEIGHT, MAP_WIDTH, GUI_HEIGHT, 'w', 'x', BORDER_TITLE_LEFT | FILL_BACKGROUND);
	/* Draw player logs */
	if (e->cLog != nullptr)
	{
		int y = MAP_HEIGHT + 1,
			x = 1;
		for (std::string log : e->cLog->logs)
		{
			Render::Puts(log, x, y++, 'w');
		}
	}

	/* Health Stats */
	//Render::Puts("HP: " + std::to_string(e->GetHealth()) + "/" + std::to_string(e->GetMaxHealth()), 1, MAP_HEIGHT + 1, 'r');

	Render::PutTitledBorder(title, 0, MAP_HEIGHT, MAP_WIDTH, GUI_HEIGHT, 'w', 'x', BORDER_TITLE_LEFT);
}

void RenderFloatingText(FloatingText *text)
{
	Render::FPuts(text->msg, text->x, text->y, text->color, text->opacity);

	text->ticks++;

	if (text->ticks % text->speed == 0)
	{
		text->y-=20;
		text->opacity = std::max(0, text->opacity - 15);
	}
}

void TakeTurn(Entity *actor)
{
	if (actor->ModEnergy(100) >= 100)
	{
		while (actor->GetEnergy() >= 100)
		{
			TurnEvent e(actor);
			actor->ModEnergy( -WorldFireEvent(&e) );
		}
	}
}
