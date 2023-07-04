#pragma once
#include "Listener.h"
class MovementListener : public Listener
{
	private:
	int FireMovementEvent(MovementEvent *e);

	public:
	MovementListener();
};

