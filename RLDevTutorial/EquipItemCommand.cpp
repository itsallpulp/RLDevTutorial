#include "EquipItemCommand.h"

EquipItemCommand::EquipItemCommand(Entity *t, Entity *i)
{
	target = t;
	item = i;
}

int EquipItemCommand::Execute()
{
	EquipItemEvent e(target, item);

    if (target == player)
    {
        while (menus.size() > 0)
        {
            PopMenu();
        }
        gameState = ON_MAP;
    }

	return WorldFireEvent(&e);
}
