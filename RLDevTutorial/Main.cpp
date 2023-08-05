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
#include "ItemInteractionListener.h"
#include "LogListener.h"
#include "MovementListener.h"
#include "RenderingListener.h"
#include "TurnListener.h"

#include "GrabItemCommand.h"
#include "MovementCommand.h"

#include "Level.h"

#include "Pathfinder.h"

#include "EntityManager.h"

#include "FloatingText.h"

#include "Menu.h"

boost::uuids::random_generator uuidGenerator;

bool quit = false;

int seed = 0;

EntityManager *actorManager, *itemManager;

CombatListener lCombat;
DeathListener lDeath;
FOVListener lFOV;
ItemInteractionListener lItems;
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
int RenderDisplay(Entity *e, int x, int y);
void RenderFloatingText(FloatingText *text);

void HandleLooking();

void TakeTurn(Entity *actor);

void PrintRuntime(void (*func)(void));

std::map<std::string, json::object> jsonCache;
std::vector<std::string> jsonCacheKey;
std::vector<FloatingText> *floatingTexts = new std::vector<FloatingText>();
std::stack<Menu *> menus;

point lookTarget;

int main(int argc, char* argv[])
{
	WeightedBag<std::string> wb = WeightedBagFromJSON(GetJson("table_monsters"));
	WeightedBag<std::string> wbItems = WeightedBagFromJSON(GetJson("table_items"));

	lookTarget = { 0,0 };

	actorManager = new EntityManager();
	itemManager = new EntityManager();
	seed = time(NULL);
	srand(seed);

	LoadColors("data/color_default.json");
	level = new Level();
	level->RoomsAndMazes();

	int playerIndex = actorManager->AddEntity("player");
	player = actorManager->GetEntity(playerIndex);

	player->cPhysics->x = 1;
	player->cPhysics->y = 1;

	level->PlaceEntity(player, actorManager);

	for (int i = 0; i < 20; ++i)
	{
		int monsterIndex = actorManager->AddEntity(wb.GetRandomValue());
		Entity *monster = actorManager->GetEntity(monsterIndex);
		level->PlaceEntity(monster, actorManager);

		int itemIndex = itemManager->AddEntity(wbItems.GetRandomValue());
		Entity *item = itemManager->GetEntity(itemIndex);
		level->PlaceEntity(item, itemManager);
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
	itemManager->RunFunc(&RenderEntity);
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

	if (menus.size() > 0)
	{
		menus.top()->Render();
	}

	if (gameState == LOOKING)
	{
		Render::Put('_', lookTarget.first + GUI_WIDTH, lookTarget.second, 'w', '~');
	}

	Render::Update();
}

int WorldFireEvent(Event *e)
{
	int r = 0;
	r += lMovement.FireEvent(e);
	r += lFOV.FireEvent(e);
	r += lLog.FireEvent(e);
	r += lCombat.FireEvent(e);
	r += lTurn.FireEvent(e);
	r += lItems.FireEvent(e);
	r += lRendering.FireEvent(e);
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
	//std::string title = "HP: " + std::to_string(e->GetHealth()) + "/" + std::to_string(e->GetMaxHealth());

	Render::PutTitledBorder("Log", GUI_WIDTH, MAP_HEIGHT, MAP_WIDTH, GUI_HEIGHT, 'w', 'x', BORDER_TITLE_CENTER | FILL_BACKGROUND);
	/* Draw player logs */
	if (e->cLog != nullptr)
	{
		int y = MAP_HEIGHT + 1,
			x = GUI_WIDTH + 1;
		for (std::string log : e->cLog->logs)
		{
			Render::Puts(log, x, y++, 'w');
		}
	}

	/* Render Player Health */
	int y = RenderDisplay(e, 0, 0);

	/* Render Visible Enemy Health */
	for (Entity *a : actorManager->GetEntities())
	{
		if (a != e && e->CanSee(a))
		{
			y = RenderDisplay(a, 0, y);
		}
	}

	y += 2;

	/* Render Names of Visible Items */
	for (Entity *e : itemManager->GetEntities())
	{
		if (!player->CanSee(e)) { continue; }
		Render::Puts(e->GetName(), 0, y++, 'y', 'x');
	}

}

int RenderDisplay(Entity *e, int x, int y)
{
	std::string name = e->GetName();
	point hp = e->GetHealthStats();
	std::string hpVals = std::to_string(hp.first) + "/" + std::to_string(hp.second);
	int hpX = (GUI_WIDTH / 2) - (hpVals.size() / 2);
	
	Render::Puts(name, x, y++, 'w');

	double n = ((double)hp.first / (double)hp.second) * GUI_WIDTH;

	for (int i = 0; i < GUI_WIDTH; ++i)
	{
		char c = 'r';
		if (i >= n) { c = 'e'; }
		Render::Put(219, x + i, y, c);
	}
	Render::Puts(hpVals, hpX, y, 'w', '~');

	return y+2;
}

void RenderFloatingText(FloatingText *text)
{
	if (text->msg.size() > 1)
	{
		Render::FPuts(text->msg, text->x, text->y, text->color, text->opacity);
	}
	else
	{
		Render::FPut(text->glyph, text->x, text->y, text->color, text->opacity);
	}
	

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
			if (gameState == IN_MENU)
			{
				Command *c = nullptr;

				while (c == nullptr)
				{
					c = menus.top()->GetCommand();
				}

				actor->ModEnergy(-(c->Execute()));
				delete c;
			}
			else if (gameState == LOOKING)
			{
				HandleLooking();
			}
			else
			{
				TurnEvent e(actor);
				actor->ModEnergy(-WorldFireEvent(&e));
			}
		}
	}
}

bool PopMenu()
{
	if (menus.size() == 0)
	{
		return true;
	}

	Menu *m = menus.top();
	menus.pop();
	delete m;
	return (menus.size() == 0);
}

void HandleLooking()
{
	while (true)
	{
		SDL_Event input;
		bool moved = false;
		while (SDL_PollEvent(&input))
		{
			if (input.type == SDL_KEYDOWN)
			{
				switch (input.key.keysym.sym)
				{
					case SDLK_LEFT:
						lookTarget.first = std::max(0, lookTarget.first - 1);
						moved = true;
						break;
					case SDLK_RIGHT:
						lookTarget.first = std::min(MAP_WIDTH - 1, lookTarget.first + 1);
						moved = true;
						break;
					case SDLK_UP:
						lookTarget.second = std::max(0, lookTarget.second - 1);
						moved = true;
						break;
					case SDLK_DOWN:
						lookTarget.second = std::min(lookTarget.second + 1, MAP_HEIGHT - 1);
						moved = true;
						break;
					case SDLK_ESCAPE:
						gameState = ON_MAP;
						return;
						break;
				}
			}
			
		}

		if (moved)
		{
			std::vector<Entity *> actors = actorManager->AllAt(lookTarget.first, lookTarget.second);
			std::vector<Entity *> items = itemManager->AllAt(lookTarget.first, lookTarget.second);
			if (actors.size() != 0 || items.size() != 0)
			{
				std::string here = "Here: ";
				for (Entity *e : actors)
				{
					here += e->GetName() + ", ";
				}
				for (Entity *e : items)
				{
					here += e->GetName() + ", ";
				}

				here = here.substr(0, here.size() - 2);

				LogEvent l(player, here);
				WorldFireEvent(&l);
			}
		}

		RenderAll();
	}
}