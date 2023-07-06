#pragma once

#include <string>

#include "Globals.h"
#include "Types.h"

#include "PhysicsComponent.h"
#include "RenderComponent.h"

class Entity
{
	private:
	std::string mName, mUUID;
	
	template<typename T>
	void AddComponent(Component **existing, json::object data)
	{
		if (*existing == nullptr)
		{
			(*existing) = new T();
		}
		((*existing))->LoadJson(data);
	}

	public:
	Entity();
	~Entity();

	Entity *Clone();

	std::string GetName();
	std::string GetUUID();

	void LoadJson(json::object data);

	PhysicsComponent *cPhysics;
	RenderComponent *cRender;
};