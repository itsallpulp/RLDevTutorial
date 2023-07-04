#pragma once
#include "Command.h"
class MovementCommand : public Command
{
	private:
	Entity *target;
	int dx, dy;

	public:
	MovementCommand(Entity *target, int dx, int dy);
	int Execute();
};

