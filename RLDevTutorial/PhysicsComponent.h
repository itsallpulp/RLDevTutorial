#pragma once
#include "Component.h"
class PhysicsComponent : public Component
{
	public:
	PhysicsComponent(int x = 0, int y = 0);
	int x, y;
	bool blocksMovement;

	void LoadJson(json::object data);
};