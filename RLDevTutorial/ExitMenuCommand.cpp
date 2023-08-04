#include "ExitMenuCommand.h"

#include "Globals.h"

ExitMenuCommand::ExitMenuCommand()
{
}

int ExitMenuCommand::Execute()
{
    
    if (PopMenu()) { gameState = ON_MAP; }

    return 0;
}
