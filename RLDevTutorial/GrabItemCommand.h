#pragma once
#include "Command.h"
class GrabItemCommand : public Command
{
	private:
	Entity *target;

	public:
	GrabItemCommand(Entity *target);
	int Execute();
};

