#include "ActorComponent.h"

ActorComponent::ActorComponent()
{
	health = 5;
	maxHealth = 5;
	baseDamage = 1;
	energy = 0;
}

void ActorComponent::LoadJson(json::object data)
{
	data = data.contains("energy") ? json::value_to<int>(data["energy"]) : energy;
	if (data.contains("maxHp"))
	{
		maxHealth = json::value_to<int>(data["maxHp"]);
		health = maxHealth;
	}
	if (data.contains("hp"))
	{
		health = json::value_to<int>(data["hp"]);
	}

	if (data.contains("baseAttack"))
	{
		json::object baseAttack = data["baseAttack"].as_object();

		baseDamage = baseAttack.contains("baseDamage") ? json::value_to<int>(baseAttack["baseDamage"]) : baseDamage;
	}
}
