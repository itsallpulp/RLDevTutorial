#pragma once
#include "Command.h"
class DropItemCommand : public Command
{
	private:
	Entity *target, *item;

	public:
	DropItemCommand(Entity *target, Entity *item);
	int Execute();
};

