#include "MenuBuilders.h"

#include <iostream>

Menu *NewInventoryMenu(Entity *e)
{
    OptionMenu *m = new OptionMenu("Inventory", 0, 0);

    int i = 97;

    for (Entity *item : e->GetInventory())
    {
        float w = std::roundf(item->GetWeight() * 100) / 100;
        std::string weight = " (" + std::to_string(w).substr(0, 3) + " lb)";
        m->AddOption(item->GetName() + weight, nullptr, i++);
    }

    return m;
}
