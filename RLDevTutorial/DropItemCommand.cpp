#include "DropItemCommand.h"

DropItemCommand::DropItemCommand(Entity *target, Entity *item)
{
    this->target = target;
    this->item = item;
}

int DropItemCommand::Execute()
{
    DropItemEvent e(target, item);
    int cost = WorldFireEvent(&e);

    if (target == player)
    {
        while (menus.size() > 0)
        {
            PopMenu();
        }
        gameState = ON_MAP;
    }

    return cost;
}
