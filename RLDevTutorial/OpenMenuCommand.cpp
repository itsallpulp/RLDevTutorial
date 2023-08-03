#include "OpenMenuCommand.h"

OpenMenuCommand::OpenMenuCommand(Menu *m)
{
	menu = m;
}

int OpenMenuCommand::Execute()
{
	gameState = IN_MENU;
	menus.push(menu);
	return 0;
}
