#pragma once

#include "Entity.h"
#include "Menu.h"
#include "OptionMenu.h"
#include "InventoryComponent.h"

extern Menu *NewInventoryMenu(Entity *e);
extern Menu *NewItemUseMenu(Entity *item, Entity *user);
extern Menu *NewEquipmentMenu(Entity *user);