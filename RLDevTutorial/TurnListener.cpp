#include "TurnListener.h"
#include "Globals.h"
#include "Command.h"
#include "MovementCommand.h"
#include "Autoexplore.h"

int TurnListener::FireTurnEvent(TurnEvent *e)
{
	std::cout << e->target->GetName() << "'s turn" << std::endl;
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
						gameState = AUTOEXPLORE;
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
	return 100;
}

TurnListener::TurnListener()
{
	RegisterListenFor(evTakeTurn);
}
