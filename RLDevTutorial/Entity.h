#pragma once

#include <string>

#include "Globals.h"

#include "PhysicsComponent.h"
#include "RenderComponent.h"

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
	RenderComponent *cRender;
};