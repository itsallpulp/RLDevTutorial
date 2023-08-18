#pragma once
#include "Component.h"

static enum EquipmentSlot {
	ARMOR = 0,
	HEAD,
	OFF_HAND,
	MAIN_HAND,
	BOOTS,
	GLOVES,
	RING,
	TWO_HAND
};

class EquippableComponent : public Component
{
	friend class Entity;

	private:
	int slot;
	int armor, damage;

	int SlotFromString(std::string slot);

	public:
	EquippableComponent();
	void LoadJson(json::object data);
};

