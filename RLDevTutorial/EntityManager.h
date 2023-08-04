#pragma once

#include "Types.h"
#include "Globals.h"
#include "Entity.h"

class EntityManager
{
	private:
	std::map<std::string, int> idMap;
	Entity entities[MAX_ENTITIES];
	byte inUse[MAX_ENTITIES];

	public:
	EntityManager();
	int AddEntity(std::string filename);
	std::set<Entity *> GetEntities();
	Entity *GetEntity(int index);
	Entity *GetEntity(std::string uuid);
	void RunFunc(void (*func)(Entity *), int filter = ON_MAP);
	Entity *At(int x, int y);
	std::vector<Entity *> AllAt(int x, int y);
	void RemoveEntity(Entity *e);
	void ChangeState(int index, int newState);
	void ChangeState(Entity *e, int newState);

	static enum EntityState {
		FREE = 0,
		ON_MAP,
		IN_INVENTORY
	};
};