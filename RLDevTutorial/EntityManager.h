#pragma once

#include "Types.h"
#include "Globals.h"
#include "Entity.h"

class EntityManager
{
	private:
	std::map<std::string, int> idMap;
	Entity entities[MAX_ENTITIES];
	bool inUse[MAX_ENTITIES];

	public:
	EntityManager();
	int AddEntity(std::string filename);
	std::set<Entity *> GetEntities();
	Entity *GetEntity(int index);
	Entity *GetEntity(std::string uuid);
};