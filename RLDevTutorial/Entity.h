#pragma once

#include <string>

#include "Globals.h"

#include "PhysicsComponent.h"

class Entity
{
	private:
	std::string mName, mUUID;

	public:
	Entity();
	~Entity();

	std::string GetName();
	std::string GetUUID();

	PhysicsComponent *cPhysics;
};