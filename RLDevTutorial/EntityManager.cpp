#include "EntityManager.h"

EntityManager::EntityManager()
{
    for (int i = 0; i < MAX_ENTITIES; ++i)
    {
        inUse[i] = false;
    }
}

int EntityManager::AddEntity(std::string filename)
{
    for (int i = 0; i < MAX_ENTITIES; ++i)
    {
        if (!inUse[i]) // Nothing really there
        {
            entities[i].Reset(); // Should be unnecessary
            entities[i].LoadJson(GetJson(filename));
            idMap[entities[i].GetUUID()] = i;
            inUse[i] = true;
            return i;
        }
    }
    return -1;
}

std::set<Entity *> EntityManager::GetEntities()
{
    std::set<Entity *> e;

    for (int i = 0; i < MAX_ENTITIES; ++i)
    {
        if (inUse[i])
        {
            e.insert(&(entities[i]));
        }
    }

    return e;
}

Entity *EntityManager::GetEntity(int index)
{
    return &(entities[index]);
}

Entity *EntityManager::GetEntity(std::string uuid)
{
    return GetEntity(idMap[uuid]);
}

void EntityManager::RunFunc(void(*func)(Entity *))
{
    for (int i = 0; i < MAX_ENTITIES; ++i)
    {
        if (inUse[i])
        {
            func(&(entities[i]));
        }
    }
}

Entity *EntityManager::At(int x, int y)
{
    for (int i = 0; i < MAX_ENTITIES; ++i)
    {
        point p = entities[i].GetXY();
        if (p.first == x && p.second == y)
        {
            return &(entities[i]);
        }
    }
    return nullptr;
}
