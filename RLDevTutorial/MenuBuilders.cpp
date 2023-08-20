#include "MenuBuilders.h"

#include <iostream>

#include "ConsumeCommand.h"
#include "ExitMenuCommand.h"
#include "OpenMenuCommand.h"
#include "DropItemCommand.h"
#include "EquipItemCommand.h"

Menu *NewInventoryMenu(Entity *e)
{
    OptionMenu *m = new OptionMenu("Inventory", 0, 0);

    int i = 97;

    for (Entity *item : e->GetInventory())
    {
        float w = std::roundf(item->GetWeight() * 100) / 100;
        std::string weight = " (" + std::to_string(w).substr(0, 3) + " lb)";

        m->AddOption(item->GetName() + " " + weight, new OpenMenuCommand(NewItemUseMenu(item, e)), i++);
    }


    return m;
}

Menu *NewItemUseMenu(Entity *item, Entity *user)
{
    OptionMenu *m = new OptionMenu(item->GetName(), 0, 0);

    m->AddOption("drop", new DropItemCommand(user, item));

    if (item->IsConsumable())
    {
        m->AddOption(item->GetConsumeVerb(), new ConsumeCommand(user, item));
    }
    if (item->IsEquippable())
    {
        m->AddOption("equip (" + item->GetEquippableSlotStr() + ")", new EquipItemCommand(user, item));
    }

    return m;
}

Menu *NewEquipmentMenu(Entity *user)
{
    OptionMenu *m = new OptionMenu("Equipment", 0, 0);
    for (int i = 0; i < NUM_SLOTS; ++i)
    {
        Entity *item = user->GetEquippedItem(i);

        if (item == nullptr) { continue; }

        std::string armorDamage = " (" + std::to_string(item->GetArmor()) + (char)31 + " " + std::to_string(item->GetDamage()) + "/)";

        m->AddOption(item->GetEquippableSlotStr() + ": " + item->GetName() + armorDamage, nullptr);
    }
    return m;
}
