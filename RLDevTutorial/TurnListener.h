#pragma once
#include "Listener.h"
class TurnListener : public Listener
{
	private:
	int FireTurnEvent(TurnEvent *e);

	int HandlePlayerTurn(TurnEvent *e);
	int HandleAITurn(TurnEvent *e);

	public:
	TurnListener();
};

