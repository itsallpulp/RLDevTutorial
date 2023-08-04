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
    return 0.0f;
}

void InventoryComponent::LoadJson(json::object data)
{
}
