#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(int x, int y)
{
	this->x = x;
	this->y = y;
	blocksMovement = false;
}

void PhysicsComponent::LoadJson(json::object data)
{
	x = data.contains("x") ? json::value_to<int>(data["x"]) : x;
	y = data.contains("y") ? json::value_to<int>(data["y"]) : y;
	blocksMovement = data.contains("blocksMovement") ? json::value_to<bool>(data["blocksMovement"]) : blocksMovement;
}
