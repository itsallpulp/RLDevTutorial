#include "TurnListener.h"
#include "Globals.h"
#include "Command.h"
#include "GrabItemCommand.h"
#include "MovementCommand.h"
#include "Autoexplore.h"
#include "EntityManager.h"
#include "Level.h"
#include "Pathfinder.h"

int TurnListener::FireTurnEvent(TurnEvent *e)
{
	if (e->target == player)
	{
		return HandlePlayerTurn(e);
	}
	else
	{
		return HandleAITurn(e);
	}
	return 0;
}

int TurnListener::HandlePlayerTurn(TurnEvent *e)
{
	int cost = 0;

	while (cost == 0)
	{
		SDL_Event input;

		Command *command = nullptr;

		while (SDL_PollEvent(&input))
		{
			if (input.type == SDL_KEYDOWN)
			{
				gameState = ON_MAP;
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
					case SDLK_x:
						if (gameState == ON_MAP) { gameState = AUTOEXPLORE; }
						break;
					case SDLK_g:
						command = new GrabItemCommand(player);
						break;
					default:
						break;
				}
			}
		}

		if (command == nullptr && gameState == AUTOEXPLORE)
		{
			command = AutoExplore();
			if (((MovementCommand*)(command))->GetDelta() == point(0, 0))
			{
				gameState = ON_MAP;
				command = nullptr;
			}
		}
		if (command != nullptr)
		{
			cost = command->Execute();
			delete command;
		}

		RenderAll();
	}
	return cost;
}

int TurnListener::HandleAITurn(TurnEvent *e)
{
	
	std::vector<Entity *> visibleActors = GetVisibleActors(e->target);

	point p = e->target->GetXY();

	if (std::find(visibleActors.begin(), visibleActors.end(), player) != visibleActors.end())
	{
		std::stack<point> path = pathfinder->GeneratePath(p, player->GetXY());
		MovementCommand *c = FollowPath(e->target, &path);
		int cost = c->Execute();
		delete c;
		return cost;
	}
	else
	{
		AddFloatingText("z", 'y', p.first, p.second);
	}
	
	return 100;
}

std::vector<Entity *> TurnListener::GetVisibleActors(Entity *e)
{
	std::vector<Entity *> v;
	
	for (Entity *a : actorManager->GetEntities())
	{
		if (a == e) { continue; }

		if (e->CanSee(a))
		{
			v.push_back(a);
		}
	}
	
	if (level->GetFOV(e->GetXY()) == fovVisible && std::find(v.begin(), v.end(), player) == v.end())
	{
		v.push_back(player);
	}
	
	return v;
}

TurnListener::TurnListener()
{
	RegisterListenFor(evTakeTurn);
}
