#pragma once
#include "Component.h"
#include <vector>

class Entity;

class InventoryComponent : public Component
{
	private:
	std::vector<Entity *> contents;

	public:
	InventoryComponent();
	float GetTotalWeight();
	float AddItem(Entity *item);
	float RemoveItem(Entity *item);
	void LoadJson(json::object data);

	friend class Entity;
};