#include "Autoexplore.h"

#include "Globals.h"
#include "Level.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Pathfinder.h"
#include "MovementCommand.h"
#include "GrabItemCommand.h"

Command *AutoExplore()
{
	for (Entity *ent : actorManager->GetEntities())
	{
		if (ent == player) { continue; }
		point p = ent->GetXY();

		if (level->GetFOV(p.first, p.second) == fovVisible)
		{
			AddFloatingText('!', 'y', p, FT_FAST);
			LogEvent logEvent(player, "You stop exploring because you see a " + ent->GetName() + ".");
			WorldFireEvent(&logEvent);
			return new MovementCommand(player, 0, 0);
		}

	}

	for (Entity *e : itemManager->GetEntities())
	{
		point p = player->GetXY();
		if (e->GetXY() == p)
		{
			return new GrabItemCommand(player);
		}

		if (player->CanSee(e))
		{
			std::stack<point> path = pathfinder->GeneratePath(player->GetXY(), e->GetXY());
			point nextStep = path.top();
			SDL_Delay(25);
			return new MovementCommand(player, nextStep.first - p.first, nextStep.second - p.second);
		}
	}


	std::vector<point> unvisited;
	for (int x = 1; x < MAP_WIDTH - 1; ++x)
	{
		for (int y = 1; y < MAP_HEIGHT - 1; ++y)
		{
			if (level->GetFOV(x, y) != fovHidden || level->GetCell(x, y)->BlocksMovement()) { continue; }
			unvisited.push_back({ x,y });
		}
	}


	if (unvisited.size() == 0) {
		std::stack<point> path = pathfinder->GeneratePath(player->GetXY(), level->GetStairs());
		point nextStep = path.top();
		SDL_Delay(25);
		point p = player->GetXY();
		return new MovementCommand(player, nextStep.first - p.first, nextStep.second - p.second);
	}


	int **path = pathfinder->CreateDijkstraMap(unvisited);


	int dx = 0, dy = 0;
	point p = player->GetXY();
	int value = path[p.first][p.second];

	if (path[p.first + 1][p.second] < value) { dx = 1; dy = 0; value = path[p.first + dx][p.second + dy]; }
	if (path[p.first - 1][p.second] < value) { dx = -1; dy = 0; value = path[p.first + dx][p.second + dy]; }
	if (path[p.first][p.second + 1] < value) { dx = 0; dy = 1;  value = path[p.first + dx][p.second + dy]; }
	if (path[p.first][p.second - 1] < value) { dx = 0; dy = -1; value = path[p.first + dx][p.second + dy]; }

	for (int i = 0; i < MAP_WIDTH; ++i)
	{
		delete path[i];
	}
	delete path;


	SDL_Delay(25);
	return new MovementCommand(player, dx, dy);
}