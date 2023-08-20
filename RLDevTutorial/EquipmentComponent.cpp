#include "EquipmentComponent.h"

#include "Entity.h"
#include "Globals.h"
#include "EntityManager.h"

EquipmentComponent::EquipmentComponent()
{
	for (int i = 0; i < NUM_SLOTS; ++i)
	{
		equippedItems[i] = nullptr;
	}
}

void EquipmentComponent::LoadJson(json::object data)
{
	if (data.contains("equipment"))
	{
		json::array a = data["equipment"].as_array();

		for (json::value v : a)
		{
			Entity *e = itemManager->GetEntity(itemManager->AddEntity(json::value_to<std::string>(v));
			if (e->GetEquippableSlot() == TWO_HAND)
			{
				equippedItems[MAIN_HAND] = e;
				equippedItems[OFF_HAND] = e;
			}
			else
			{
				equippedItems[e->GetEquippableSlot()] = e;
			}
		}
	}
}
