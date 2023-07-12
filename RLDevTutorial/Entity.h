#pragma once

#include <string>

#include "Globals.h"
#include "Types.h"

#include "FOVComponent.h"
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
	Entity(std::string fileToLoad);
	~Entity();

	Entity *Clone();
	void Copy(Entity *other);
	void Reset();

	std::string GetName();
	std::string GetUUID();

	void LoadJson(json::object data);

	FOVComponent *cFOV;
	PhysicsComponent *cPhysics;
	point GetXY();
	bool BlocksMovement();
	bool BlocksVision();

	RenderComponent *cRender;
};