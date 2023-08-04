#include "OpenMenuCommand.h"

OpenMenuCommand::OpenMenuCommand(Menu *m)
{
	menu = m;
}

int OpenMenuCommand::Execute()
{
	gameState = IN_MENU;
	menus.push(menu);

	std::cout << menus.size() << " menus open" << std::endl;

	return 0;
}
