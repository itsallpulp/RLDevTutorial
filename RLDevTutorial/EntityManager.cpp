#include "EntityManager.h"

EntityManager::EntityManager()
{
    for (int i = 0; i < MAX_ENTITIES; ++i)
    {
        inUse[i] = FREE;
    }
}

int EntityManager::AddEntity(std::string filename)
{
    for (int i = 0; i < MAX_ENTITIES; ++i)
    {
        if (inUse[i] == FREE) // Nothing really there
        {
            entities[i].Reset(); // Should be unnecessary
            entities[i].LoadJson(GetJson(filename));
            idMap[entities[i].GetUUID()] = i;
            inUse[i] = ON_MAP;
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
        if (inUse[i] == ON_MAP)
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

void EntityManager::RunFunc(void(*func)(Entity *), int filter)
{
    for (int i = 0; i < MAX_ENTITIES; ++i)
    {
        if (inUse[i] == filter)
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

std::vector<Entity *> EntityManager::AllAt(int x, int y)
{
    std::vector<Entity *> e;

    for (int i = 0; i < MAX_ENTITIES; ++i)
    {
        if (inUse[i] == ON_MAP)
        {
            point p = entities[i].GetXY();
            if (p.first == x && p.second == y)
            {
                e.push_back(&entities[i]);
            }
        }
    }

    return e;
}

void EntityManager::RemoveEntity(Entity *e)
{
    for (int i = 0; i < MAX_ENTITIES; ++i)
    {
        if (entities[i].GetUUID() == e->GetUUID())
        {
            entities[i].Reset();
            inUse[i] = FREE;
            flags[i] = 0;
            return;
        }
    }
}

void EntityManager::ChangeState(int index, int newState)
{
    inUse[index] = newState;
}

void EntityManager::ChangeState(Entity *e, int newState)
{
    for (int i = 0; i < MAX_ENTITIES; ++i)
    {
        if (entities[i].GetUUID() == e->GetUUID())
        {
            inUse[i] = newState;
            return;
        }
    }
}

void EntityManager::Clear()
{
    for (int i = 0; i < MAX_ENTITIES; ++i)
    {
        if (!(flags[i] & PERMANENT))
        {
            flags[i] = 0;
            entities[i].Reset();
            inUse[i] = FREE;
        }
    }
}

void EntityManager::ToggleFlag(Entity *e, int flag)
{
    for (int i = 0; i < MAX_ENTITIES; ++i)
    {
        if (e->GetUUID() == entities[i].GetUUID())
        {
            if (flags[i] & flag) { flags[i] &= ~(flag); }
            else { flags[i] |= flag; }
        }
    }
}
