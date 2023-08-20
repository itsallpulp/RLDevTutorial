#pragma once
#include "Component.h"

#include "EquippableComponent.h"

class Entity;

class EquipmentComponent : public Component
{
	friend class Entity;

	private:
	Entity *equippedItems[NUM_SLOTS];

	public:
	EquipmentComponent();
	void LoadJson(json::object data);
};

