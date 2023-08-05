#pragma once
#include "Component.h"

typedef struct Zap {
	int amount;
	std::string type, targetType;
};

class ConsumableComponent : public Component
{
	friend class Entity;

	public:
	ConsumableComponent();
	void LoadJson(json::object data);

	private:
	void LoadZap(json::object data);
	int healAmount;
	std::vector<Zap> zaps;
	std::string verb;
};

