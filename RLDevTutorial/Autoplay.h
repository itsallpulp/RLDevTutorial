#pragma once

#include "Globals.h"
#include "Types.h"
#include "MovementCommand.h"
#include "Entity.h"

class Autoplay
{
	private:
	Entity *target;

	public:
	Autoplay(Entity *target);
	void Run();
};