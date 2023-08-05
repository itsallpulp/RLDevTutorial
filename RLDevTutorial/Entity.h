#pragma once

#include <string>

#include "Globals.h"
#include "Types.h"

#include "ComponentsList.h"

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

	bool IsConsumable();

	ActorComponent *cActor;
	point GetHealthStats();
	int GetHealth();
	int GetMaxHealth();
	int Heal(int amount);
	bool IsAlive();
	void TakeDamage(int damage);
	int GetEnergy();
	int ModEnergy(int d);

	FOVComponent *cFOV;

	ConsumableComponent *cConsumable;
	bool HealsOnConsume();
	int GetConsumeHealAmount();
	std::string GetConsumeVerb();
	bool ZapsOnConsume();
	std::vector<Zap> GetZaps();

	InventoryComponent *cInventory;
	bool AddItem(Entity *item);
	void RemoveItem(Entity *item);
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