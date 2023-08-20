#pragma once
#include "Command.h"
class EquipItemCommand : public Command
{
	private:
	Entity *item, *target;

	public:
	EquipItemCommand(Entity *t, Entity *i);
	int Execute();
};