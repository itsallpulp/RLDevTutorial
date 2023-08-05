#include "LookCommand.h"
#include "Entity.h"

LookCommand::LookCommand()
{
}

int LookCommand::Execute()
{
    gameState = LOOKING;
    lookTarget = player->GetXY();
    return 0;
}
