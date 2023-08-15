#pragma once
#include "Component.h"
class PhysicsComponent : public Component
{
	public:
	PhysicsComponent(int x = 0, int y = 0);
	int x, y;
	bool blocksMovement, blocksVision, allowDescend;
	float weight;

	void LoadJson(json::object data);
};