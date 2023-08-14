#pragma once
#include "Component.h"
class StatusEffectsComponent : public Component
{
	friend class Entity;
	private:
	std::map<std::string, int> effects;

	public:
	StatusEffectsComponent();
	void LoadJson(json::object data);
};

