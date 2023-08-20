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
		bool permanent = data.contains("makePermanent");

		json::array a = data["equipment"].as_array();

		for (json::value v : a)
		{
			Entity *e = itemManager->GetEntity(itemManager->AddEntity(json::value_to<std::string>(v)));
			itemManager->ChangeState(e, EntityManager::IN_INVENTORY);
			if (permanent) { itemManager->ToggleFlag(e, EntityManager::PERMANENT); }


			if (e->GetEquippableSlot() == TWO_HAND)
			{
				if (equippedItems[MAIN_HAND] != nullptr) { itemManager->RemoveEntity(equippedItems[MAIN_HAND]); }
				if (equippedItems[OFF_HAND] != nullptr) { itemManager->RemoveEntity(equippedItems[OFF_HAND]); }

				equippedItems[MAIN_HAND] = e;
				equippedItems[OFF_HAND] = e;
			}
			else
			{
				if (equippedItems[e->GetEquippableSlot()] != nullptr) { itemManager->RemoveEntity(equippedItems[e->GetEquippableSlot()]); }
				equippedItems[e->GetEquippableSlot()] = e;
			}
		}
	}
}
