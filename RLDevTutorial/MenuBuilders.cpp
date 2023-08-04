#include "MenuBuilders.h"

#include <iostream>

#include "ConsumeCommand.h"
#include "ExitMenuCommand.h"

Menu *NewInventoryMenu(Entity *e)
{
    OptionMenu *m = new OptionMenu("Inventory", 0, 0);

    int i = 97;

    for (Entity *item : e->GetInventory())
    {
        float w = std::roundf(item->GetWeight() * 100) / 100;
        std::string weight = " (" + std::to_string(w).substr(0, 3) + " lb)";

        Command *c = nullptr;

        if (item->IsConsumable())
        {
            std::cout << "Consumable" << std::endl;
            c = new ConsumeCommand(e, item);
        }

        m->AddOption(item->GetName() + weight, c, i++);
    }


    return m;
}
