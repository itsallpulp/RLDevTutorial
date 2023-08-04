#pragma once
#include "Component.h"
class HealComponent : public Component
{
	friend class Entity;
	public:
	HealComponent();
	void LoadJson(json::object data);

	private:
	int amount;
	bool activateOnConsume;
};

