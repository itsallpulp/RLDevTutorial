#pragma once
#include "Component.h"
class ConsumableComponent : public Component
{
	friend class Entity;

	public:
	ConsumableComponent();
	void LoadJson(json::object data);

	private:
	int healAmount;
	std::string verb;
};

