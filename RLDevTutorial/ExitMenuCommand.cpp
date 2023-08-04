#include "ExitMenuCommand.h"

#include "Globals.h"

ExitMenuCommand::ExitMenuCommand()
{
}

int ExitMenuCommand::Execute()
{
    Menu *m = menus.top();
    menus.pop();

    delete m;

    if (menus.size() == 0) { gameState = ON_MAP; }

    return 0;
}
