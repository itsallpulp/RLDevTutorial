#pragma once
#include "Command.h"
class ConsumeCommand : public Command
{
	private:
	Entity *consumer, *consumed;

	public:
	ConsumeCommand(Entity *consumer, Entity *consumed);
	int Execute();
};