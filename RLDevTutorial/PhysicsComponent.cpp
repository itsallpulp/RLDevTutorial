#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(int x, int y)
{
	this->x = x;
	this->y = y;
	blocksMovement = false;
	blocksVision = false;
	weight = 0;
	allowDescend = false;
}

void PhysicsComponent::LoadJson(json::object data)
{
	x = data.contains("x") ? json::value_to<int>(data["x"]) : x;
	y = data.contains("y") ? json::value_to<int>(data["y"]) : y;
	blocksMovement = data.contains("blocksMovement") ? json::value_to<bool>(data["blocksMovement"]) : blocksMovement;
	blocksVision = data.contains("blocksVision") ? json::value_to<bool>(data["blocksVision"]) : blocksVision;
	allowDescend = data.contains("allowDescend") ? json::value_to<bool>(data["allowDescend"]) : allowDescend;
	weight = data.contains("weight") ? json::value_to<float>(data["weight"]) : weight;
}
