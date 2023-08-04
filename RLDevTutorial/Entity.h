#pragma once

#include <string>

#include "Globals.h"
#include "Types.h"

#include "ActorComponent.h"
#include "FOVComponent.h"
#include "InventoryComponent.h"
#include "LogComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"

#include "Event.h"

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

	ActorComponent *cActor;
	point GetHealthStats();
	int GetHealth();
	int GetMaxHealth();
	bool IsAlive();
	void TakeDamage(int damage);
	int GetEnergy();
	int ModEnergy(int d);

	FOVComponent *cFOV;

	InventoryComponent *cInventory;
	bool AddItem(Entity *item);
	std::vector<Entity *> GetInventory();

	PhysicsComponent *cPhysics;
	point GetXY();
	void SetXY(int x, int y);
	bool BlocksMovement();
	bool BlocksVision();
	float GetWeight();

	RenderComponent *cRender;

	LogComponent *cLog;
	void SendLog(std::string message);

	void FigureDamage(DamageEvent *e);

	bool CanSee(Entity *other);

};