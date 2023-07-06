#pragma once
#include "Listener.h"

#include "Level.h"

class MovementListener : public Listener
{
	private:
	int FireMovementEvent(MovementEvent *e);

	public:
	MovementListener();
};