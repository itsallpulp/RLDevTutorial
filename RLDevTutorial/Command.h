#pragma once

#include "Globals.h"
#include "Event.h"

class Entity;

class Command
{
	public:
	Command();
	virtual int Execute() { return 0; }
};