#pragma once
#include "Command.h"

#include "Menu.h"
#include "Globals.h"

class OpenMenuCommand : public Command
{
	private:
	Menu *menu;

	public:
	OpenMenuCommand(Menu *m);
	int Execute();
};

