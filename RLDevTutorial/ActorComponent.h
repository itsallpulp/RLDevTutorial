#pragma once
#include "Component.h"
class ActorComponent : public Component
{
	public:
	ActorComponent();
	void LoadJson(json::object data);

	int health, maxHealth;
};