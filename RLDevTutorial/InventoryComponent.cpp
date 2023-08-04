#include "InventoryComponent.h"

InventoryComponent::InventoryComponent()
{
}

float InventoryComponent::GetTotalWeight()
{
    return 0.0f;
}

float InventoryComponent::AddItem(Entity *item)
{
    contents.push_back(item);
    return GetTotalWeight();
}

float InventoryComponent::RemoveItem(Entity *item)
{

    for (auto it = contents.begin(); it != contents.end(); ++it)
    {
        if ((*it) == item)
        {
            contents.erase(it);
            return GetTotalWeight();
        }
    }

    return GetTotalWeight();
}

void InventoryComponent::LoadJson(json::object data)
{
}
