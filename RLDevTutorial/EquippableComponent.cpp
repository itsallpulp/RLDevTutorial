#include "EquippableComponent.h"

int EquippableComponent::SlotFromString(std::string slot)
{
	if (slot == "armor") { return ARMOR; }
	if (slot == "head") { return HEAD; }
	if (slot == "off hand") { return OFF_HAND; }
	if (slot == "main hand") { return MAIN_HAND; }
	if (slot == "feet") { return BOOTS; }
	if (slot == "hands") { return GLOVES; }
	if (slot == "ring") { return RING; }
	if (slot == "two hand") { return TWO_HAND; }

	return 0;
}

EquippableComponent::EquippableComponent()
{
	armor = 0;
	damage = 0;
	slot = ARMOR;
}

void EquippableComponent::LoadJson(json::object data)
{
	armor = data.contains("armor") ? json::value_to<int>(data["armor"]) : armor;
	damage = data.contains("damage") ? json::value_to<int>(data["damage"]) : damage;
	try {
		slot = data.contains("slot") ? json::value_to<int>(data["slot"]) : slot;
	}
	catch (...){
		slot = SlotFromString(json::value_to<std::string>(data["slot"]));
	}
}
