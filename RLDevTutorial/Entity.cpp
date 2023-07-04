#include "Entity.h"

Entity::Entity()
{
    mName = "";
    mUUID = GenerateUUID();

    cPhysics = nullptr;
}

Entity::~Entity()
{
    delete cPhysics;
}

std::string Entity::GetName()
{
    return mName;
}

std::string Entity::GetUUID()
{
    return mUUID;
}
